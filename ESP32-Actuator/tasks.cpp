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
  
  // Setup actuator pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("[Actuator] Ready - Waiting for commands");
  
  String command;
  for (;;) {
    // Check for commands in queue
    if (xQueueReceive(commandQueue, &command, pdMS_TO_TICKS(100)) == pdTRUE) {
      Serial.println("[Actuator] Received command: " + command);
      
      // Parse JSON command
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, command);
      
      if (error) {
        Serial.println("[Actuator] Failed to parse command JSON");
        continue;
      }
      
      const char* type = doc["type"];
      
      if (strcmp(type, "relay") == 0) {
        bool state = doc["state"];
        digitalWrite(RELAY_PIN, state ? HIGH : LOW);
        Serial.println("[Actuator] Relay " + String(state ? "ON" : "OFF"));
      }
      else if (strcmp(type, "led") == 0) {
        bool state = doc["state"];
        digitalWrite(LED_PIN, state ? HIGH : LOW);
        Serial.println("[Actuator] LED " + String(state ? "ON" : "OFF"));
      }
      else if (strcmp(type, "neopixel") == 0) {
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
