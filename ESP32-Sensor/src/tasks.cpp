/**
 * @file tasks.cpp
 * @brief FreeRTOS Tasks Implementation
 * 
 * Implements three concurrent tasks for multi-threaded operation:
 * 
 * 1. TaskSensors - Reads DHT20 sensor data (Core 0, Priority 1)
 * 2. TaskUI - Updates NeoPixel LED status (Core 0, Priority 1)
 * 3. TaskMQTT - Manages MQTT communication (Core 1, Priority 2)
 * 
 * Tasks communicate via:
 * - FreeRTOS queues (telemetry data)
 * - Event groups (connection state)
 * - Mutexes (I2C bus access)
 */

#include "tasks.h"
#include "globals.h"
#include "neopixel_handler.h"
#include "mqtt_handler.h"
#include <Arduino.h>

/**
 * @brief Sensor reading task (FreeRTOS)
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Continuously reads DHT20 temperature/humidity sensor:
 * - Reads sensor every 1 second with mutex protection
 * - Validates data quality (range checks, NaN detection)
 * - Queues valid telemetry for MQTT transmission
 * - Tracks read/error statistics
 * 
 * Pinned to Core 0, Priority 1
 */
void TaskSensors(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(1000);  // 1-second read interval
  
  Serial.println("[Sensors] Task started - Initializing DHT20...");
  
  // ===== SENSOR INITIALIZATION =====
  // Simple DHT20 initialization with mutex protection
  if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
    dht20.begin();
    xSemaphoreGive(i2cMutex);
    Serial.println("[Sensors] ✓ DHT20 initialized");
  }
  
  sensorState.initialized = true;
  Serial.println("[Sensors] Sensor ready - Reading every 1 second");
  
  // ===== MAIN SENSOR LOOP =====
  for (;;) {
    TelemetryData data = {0};
    data.valid = false;
    data.quality = 100;  // Default to perfect quality
    float temperature = 0;
    float humidity = 0;
    
    // ===== READ DHT20 WITH MUTEX PROTECTION =====
    if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(200)) == pdTRUE) {
      // Perform sensor read
      dht20.read();
      temperature = dht20.getTemperature();
      humidity = dht20.getHumidity();
      xSemaphoreGive(i2cMutex);
      
      // ===== VALIDATE SENSOR DATA =====
      // Check for NaN (Not a Number) indicating read failure
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("[Sensors] Failed to read from DHT sensor!");
        temperature = -1;
        humidity = -1;
        data.valid = false;
        data.quality = 0;  // Zero quality for failed read
        sensorState.errorCount++;
      } else {
        // ===== SUCCESSFUL READ =====
        data.temperature = temperature;
        data.humidity = humidity;
        data.valid = true;
        sensorState.readCount++;
        
        // Log successful reading
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
      }
    } else {
      // ===== MUTEX TIMEOUT =====
      // Another task is using I2C bus
      Serial.println("[Sensors] ✗ Mutex timeout (I2C busy)");
      temperature = -1;
      humidity = -1;
      data.valid = false;
      data.quality = 0;
      sensorState.errorCount++;
    }
    
    // ===== POPULATE TELEMETRY DATA =====
    data.temperature = temperature;
    data.humidity = humidity;
    data.heap = ESP.getFreeHeap();  // Free heap memory
    data.uptime = millis() / 1000;   // Uptime in seconds
    
    // ===== QUEUE TELEMETRY FOR MQTT TASK =====
    // Only queue if WiFi is connected (prevents queue overflow)
    EventBits_t bits = xEventGroupGetBits(connectionEvents);
    if (bits & WIFI_CONNECTED_BIT) {
      // Try to send to queue with timeout
      if (xQueueSend(telemetryQueue, &data, pdMS_TO_TICKS(10)) != pdTRUE) {
        // Queue full - drop oldest item and retry
        TelemetryData dummy;
        xQueueReceive(telemetryQueue, &dummy, 0);
        xQueueSend(telemetryQueue, &data, 0);
        Serial.println("[Sensors] ⚠ Queue full, dropped oldest reading");
      }
    }
    
    vTaskDelay(xDelay);  // Wait 1 second before next read
  }
}

/**
 * @brief User interface update task (FreeRTOS)
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Updates NeoPixel LED color based on system connection state.
 * Provides visual feedback to user every 500ms.
 * 
 * Pinned to Core 0, Priority 1
 */
void TaskUI(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(UI_UPDATE_INTERVAL_MS);  // 500ms
  
  for (;;) {
    updateNeoPixel();  // Update LED color based on connection state
    vTaskDelay(xDelay);
  }
}

/**
 * @brief MQTT communication task (FreeRTOS)
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Manages MQTT connection and telemetry publishing:
 * - Waits for MQTT connection using event groups
 * - Processes telemetry queue and publishes to broker
 * - Handles automatic reconnection on connection loss
 * - Prevents queue overflow with batch processing
 * 
 * Pinned to Core 1 (separate from sensors), Priority 2 (high)
 */
void TaskMQTT(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(MQTT_LOOP_INTERVAL_MS);  // 100ms
  TelemetryData data;
  uint32_t publishCount = 0;
  uint32_t publishErrors = 0;
  
  for (;;) {
    // ===== WAIT FOR MQTT CONNECTION =====
    // Block until MQTT_CONNECTED_BIT is set by connectMQTT()
    EventBits_t bits = xEventGroupWaitBits(
      connectionEvents,
      MQTT_CONNECTED_BIT,
      pdFALSE,  // Don't clear bit on exit
      pdTRUE,   // Wait for all bits (only one bit in this case)
      xDelay    // Timeout
    );
    
    if (bits & MQTT_CONNECTED_BIT) {
      // ===== MQTT IS CONNECTED =====
      mqttClient.loop();  // Process MQTT client (callbacks, keepalive)
      
      // ===== PROCESS TELEMETRY QUEUE =====
      // Publish all pending telemetry from queue
      uint32_t processed = 0;
      while (xQueueReceive(telemetryQueue, &data, pdMS_TO_TICKS(10)) == pdTRUE) {
        // Only publish valid data or critical errors (quality=0)
        if (data.valid || data.quality == 0) {
          if (publishTelemetry(data)) {
            publishCount++;
            processed++;
            
            // Log progress every 20 successful publishes
            if (publishCount % 20 == 0) {
              Serial.printf("[MQTT] Published %d messages (errors: %d)\n", 
                           publishCount, publishErrors);
            }
          } else {
            publishErrors++;
            Serial.println("[MQTT] ✗ Publish failed");
          }
        }
        
        // Prevent blocking too long - process max 5 items per loop
        if (processed >= 5) break;
      }
      
      // ===== CHECK CONNECTION HEALTH =====
      if (!mqttClient.connected()) {
        // Connection lost - clear event bit and attempt reconnect
        xEventGroupClearBits(connectionEvents, MQTT_CONNECTED_BIT);
        mqttConnected = false;
        Serial.println("[MQTT] Connection lost, reconnecting...");
        vTaskDelay(pdMS_TO_TICKS(5000));  // Wait 5s before retry
        connectMQTT();
      }
    } else {
      // ===== MQTT NOT CONNECTED =====
      // Try to connect if WiFi is up
      if (wifiConnected && !mqttConnected) {
        connectMQTT();
      }
    }
    
    vTaskDelay(xDelay);  // Wait 100ms before next loop
  }
}
