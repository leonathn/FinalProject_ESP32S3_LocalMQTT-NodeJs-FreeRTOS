/**
 * @file mqtt_handler.cpp
 * @brief MQTT Connection and GPIO Command Handling Implementation
 * 
 * Manages MQTT broker connection, GPIO command subscriptions, status publishing,
 * and incoming command processing for the actuator device.
 */

#include "mqtt_handler.h"
#include "globals.h"
#include "neopixel_handler.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @brief Connect to MQTT broker
 * 
 * Establishes MQTT connection using configured server and port.
 * On success:
 * - Subscribes to GPIO control topic
 * - Publishes initial status message
 * - Sets event group bit for other tasks
 */
void connectMQTT() {
  // Require WiFi connection and MQTT server configured
  if (!wifiConnected || mqttServer.length() == 0) return;
  
  // Configure MQTT client
  mqttClient.setServer(mqttServer.c_str(), mqttPort);
  mqttClient.setCallback(mqttCallback);  // Set message handler
  mqttClient.setBufferSize(512);  // Increase buffer for larger messages
  
  Serial.print("[MQTT] Connecting to: " + mqttServer + ":" + String(mqttPort));
  
  // Attempt connection with device ID as client ID
  if (mqttClient.connect(deviceId.c_str())) {
    // ===== CONNECTION SUCCESS =====
    mqttConnected = true;
    xEventGroupSetBits(connectionEvents, MQTT_CONNECTED_BIT);  // Notify other tasks
    Serial.println(" Connected!");
    
    // Subscribe to GPIO control topic
    String gpioTopic = "device/" + deviceId + "/gpio/set";
    mqttClient.subscribe(gpioTopic.c_str());
    Serial.println("[MQTT] Subscribed to: " + gpioTopic);
    
    // Publish initial status
    publishStatus();
    updateNeoPixel();  // Update LED to green
  } else {
    // ===== CONNECTION FAILED =====
    Serial.println(" Failed, rc=" + String(mqttClient.state()));
  }
}

/**
 * @brief MQTT message callback handler
 * @param topic Topic name of received message
 * @param payload Raw message payload bytes
 * @param length Payload length in bytes
 * 
 * Processes incoming MQTT messages for GPIO control.
 * Expected JSON format: {"gpio": 1-8, "state": 0/1}
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  
  Serial.println("[MQTT RX] " + String(topic) + ": " + payloadStr);
  
  // Parse JSON payload
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payloadStr);
  if (error) {
    Serial.println("[MQTT] JSON parse error: " + String(error.c_str()));
    return;
  }
  
  // Extract GPIO command
  if (doc.containsKey("gpio") && doc.containsKey("state")) {
    int gpioChannel = doc["gpio"];  // 1-8
    int state = doc["state"];        // 0=OFF, 1=ON
    
    // Validate GPIO channel
    if (gpioChannel >= 1 && gpioChannel <= 8) {
      int gpioPin = GPIO_PINS[gpioChannel - 1];  // Convert to array index
      
      // Set GPIO state
      digitalWrite(gpioPin, state ? HIGH : LOW);
      
      // Update internal state tracking
      switch (gpioChannel) {
        case 1: currentGPIOState.ch1 = state; break;
        case 2: currentGPIOState.ch2 = state; break;
        case 3: currentGPIOState.ch3 = state; break;
        case 4: currentGPIOState.ch4 = state; break;
        case 5: currentGPIOState.ch5 = state; break;
        case 6: currentGPIOState.ch6 = state; break;
        case 7: currentGPIOState.ch7 = state; break;
        case 8: currentGPIOState.ch8 = state; break;
      }
      
      Serial.println("[GPIO] Channel " + String(gpioChannel) + " -> " + String(state ? "ON" : "OFF"));
      
      // Publish updated status immediately
      publishStatus();
    } else {
      Serial.println("[GPIO] Invalid channel: " + String(gpioChannel));
    }
  }
}

/**
 * @brief Publish device status to MQTT
 * 
 * Publishes JSON status message containing:
 * - Online state (true)
 * - Current GPIO states for all 8 channels
 * - System metrics (heap, uptime)
 * - IP address and RSSI
 * 
 * Topic: devices/<device_id>/status
 */
void publishStatus() {
  if (!mqttConnected) return;
  
  // Build JSON status message
  String topic = "devices/" + deviceId + "/status";
  JsonDocument doc;
  
  doc["online"] = true;
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  doc["heap"] = ESP.getFreeHeap();
  doc["uptime"] = millis() / 1000;
  
  // Add GPIO states
  JsonObject gpio = doc.createNestedObject("gpio");
  gpio["ch1"] = currentGPIOState.ch1;
  gpio["ch2"] = currentGPIOState.ch2;
  gpio["ch3"] = currentGPIOState.ch3;
  gpio["ch4"] = currentGPIOState.ch4;
  gpio["ch5"] = currentGPIOState.ch5;
  gpio["ch6"] = currentGPIOState.ch6;
  gpio["ch7"] = currentGPIOState.ch7;
  gpio["ch8"] = currentGPIOState.ch8;
  
  doc["ts"] = millis();
  
  // Serialize and publish
  char buffer[512];
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer, true);  // retained=true
}
