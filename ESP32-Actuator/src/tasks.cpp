/*
 * FreeRTOS Tasks Implementation - Actuator Version
 */

#include "tasks.h"
#include "globals.h"
#include "neopixel_handler.h"
#include "mqtt_handler.h"
#include <Arduino.h>
#include <ArduinoJson.h>

// Example actuator pins
#define RELAY_PIN 13
#define LED_PIN 12

void TaskActuator(void *pvParameters) {
  Serial.println("[Actuator] Task started");
  
  // Setup all 8 GPIO output pins
  Serial.println("[GPIO] Initializing 8 output pins:");
  for (int i = 0; i < 8; i++) {
    pinMode(gpioOutputPins[i], OUTPUT);
    digitalWrite(gpioOutputPins[i], LOW);
    gpioStates[i] = false;
    Serial.println("  GPIO" + String(i + 1) + " -> Pin " + String(gpioOutputPins[i]));
  }
  
  Serial.println("[Actuator] Ready - Waiting for commands");
  
  String command;
  for (;;) {
    // Check for commands in queue
    if (xQueueReceive(commandQueue, &command, pdMS_TO_TICKS(100)) == pdTRUE) {
      Serial.println("[Actuator] ========================================");
      Serial.println("[Actuator] Received command: " + command);
      
      // Parse JSON command
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, command);
      
      if (error) {
        Serial.println("[Actuator] ✗ Failed to parse JSON: " + String(error.c_str()));
        continue;
      }
      Serial.println("[Actuator] ✓ JSON parsed successfully");
      
      const char* type = doc["type"];
      Serial.println("[Actuator] Command type: " + String(type));
      
      if (strcmp(type, "gpio") == 0 || strcmp(type, "relay") == 0) {
        // GPIO ON/OFF command: {"type":"gpio", "pin":1, "state":true}
        int pin = doc["pin"];  // 1-8
        bool state = doc["state"];
        
        Serial.println("[Actuator] GPIO pin: " + String(pin) + ", state: " + String(state));
        
        if (pin >= 1 && pin <= 8) {
          int idx = pin - 1;
          uint8_t physicalPin = gpioOutputPins[idx];
          Serial.println("[GPIO] Setting physical pin " + String(physicalPin) + " to " + String(state ? "HIGH" : "LOW"));
          digitalWrite(physicalPin, state ? HIGH : LOW);
          gpioStates[idx] = state;
          Serial.println("[GPIO" + String(pin) + "] ✓ Pin " + String(physicalPin) + " -> " + String(state ? "ON" : "OFF"));
        } else {
          Serial.println("[Actuator] ✗ Invalid GPIO pin: " + String(pin) + " (must be 1-8)");
        }
      }
      else if (strcmp(type, "neopixel") == 0 || strcmp(type, "led") == 0) {
        uint8_t r = doc["color"]["r"];
        uint8_t g = doc["color"]["g"];
        uint8_t b = doc["color"]["b"];
        pixel.setPixelColor(0, pixel.Color(r, g, b));
        pixel.show();
        Serial.println("[Actuator] NeoPixel color updated");
      }
      else {
        Serial.println("[Actuator] Unknown command type: " + String(type));
      }
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void TaskUI(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(UI_UPDATE_INTERVAL_MS);
  
  for (;;) {
    updateNeoPixel();
    vTaskDelay(xDelay);
  }
}

void TaskMQTT(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(MQTT_LOOP_INTERVAL_MS);
  unsigned long lastTelemetry = 0;
  const unsigned long telemetryInterval = 10000; // Publish telemetry every 10 seconds
  
  for (;;) {
    // Wait for MQTT connection
    EventBits_t bits = xEventGroupWaitBits(
      connectionEvents,
      MQTT_CONNECTED_BIT,
      pdFALSE,
      pdTRUE,
      xDelay
    );
    
    if (bits & MQTT_CONNECTED_BIT) {
      mqttClient.loop();
      
      // Publish telemetry periodically
      if (millis() - lastTelemetry >= telemetryInterval) {
        publishTelemetry();
        lastTelemetry = millis();
      }
      
      // Reconnect if needed
      if (!mqttClient.connected()) {
        xEventGroupClearBits(connectionEvents, MQTT_CONNECTED_BIT);
        mqttConnected = false;
        updateNeoPixel();
        
        connectMQTT();
      }
    } else {
      // Not connected, try to connect if WiFi is up
      EventBits_t wifiBits = xEventGroupGetBits(connectionEvents);
      if (wifiBits & WIFI_CONNECTED_BIT) {
        if (mqttServer.length() > 0) {
          connectMQTT();
        }
      }
    }
    
    vTaskDelay(xDelay);
  }
}
