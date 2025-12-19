/**
 * @file mqtt_handler.cpp
 * @brief MQTT Connection and Message Handling Implementation
 * 
 * Manages MQTT broker connection, topic subscriptions, message publishing,
 * and incoming command processing. Implements device-to-server communication
 * using JSON-formatted MQTT messages.
 */

#include "mqtt_handler.h"
#include "globals.h"
#include "neopixel_handler.h"
#include "diagnostics.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @brief Connect to MQTT broker
 * 
 * Establishes MQTT connection using configured server and port.
 * On success:
 * - Subscribes to command and config topics
 * - Publishes initial status and pairing messages
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
    
    // Subscribe to command and configuration topics
    String configTopic = "devices/" + deviceId + "/config";
    String cmdTopic = "devices/" + deviceId + "/cmd";
    mqttClient.subscribe(configTopic.c_str());
    mqttClient.subscribe(cmdTopic.c_str());
    
    // Publish initial messages
    publishStatus();   // Announce device online
    publishPairing();  // Send pairing token
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
 * Processes incoming MQTT messages from subscribed topics.
 * Parses JSON payloads and executes corresponding commands.
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Convert topic and payload to strings
  String topicStr = String(topic);
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  
  Serial.println("[MQTT] " + topicStr + ": " + payloadStr);
  
  // Parse JSON payload
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payloadStr);
  if (error) {
    Serial.println("[MQTT] JSON parse error");
    return;
  }
  
  // ===== HANDLE COMMANDS =====
  if (topicStr.endsWith("/cmd")) {
    const char* cmd = doc["cmd"];
    
    // Reboot command
    if (strcmp(cmd, "reboot") == 0) {
      Serial.println("[CMD] Rebooting...");
      delay(1000);
      ESP.restart();
    } 
    // Run diagnostics command
    else if (strcmp(cmd, "diagnostics") == 0) {
      runDiagnostics();
    }
  }
}

/**
 * @brief Publish device online status
 * 
 * Publishes retained status message with device information.
 * Retained messages are delivered to new subscribers immediately.
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
  doc["wifiMode"] = apMode ? "AP" : "STA";
  doc["ts"] = millis();
  
  // Serialize and publish (retained)
  char buffer[256];
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer, true);  // true = retained
}

/**
 * @brief Publish telemetry data
 * @param data Reference to telemetry data structure
 * @return True if publish succeeded, false otherwise
 * 
 * Publishes sensor readings and system metrics to MQTT broker.
 * Topic: devices/<device_id>/telemetry
 */
bool publishTelemetry(TelemetryData &data) {
  // Check connection before publishing
  if (!mqttConnected || !mqttClient.connected()) {
    return false;
  }
  
  // Build JSON telemetry message
  String topic = "devices/" + deviceId + "/telemetry";
  JsonDocument doc;
  doc["tC"] = data.temperature;      // Temperature in Celsius
  doc["rh"] = data.humidity;          // Relative humidity %
  doc["heap"] = data.heap;            // Free heap bytes
  doc["uptime"] = data.uptime;        // Uptime in seconds
  doc["quality"] = data.quality;      // Data quality 0-100%
  doc["valid"] = data.valid;          // Overall data validity
  doc["ts"] = millis();               // Timestamp
  
  // Serialize and publish (not retained - high frequency data)
  char buffer[256];
  serializeJson(doc, buffer);
  bool published = mqttClient.publish(topic.c_str(), buffer);
  
  return published;
}

/**
 * @brief Publish device pairing token
 * 
 * Sends pairing token for server-side device registration/approval.
 * Server can use this to authenticate device during first connection.
 * Topic: devices/<device_id>/pair
 */
void publishPairing() {
  if (!mqttConnected) return;
  
  // Build JSON pairing message
  String topic = "devices/" + deviceId + "/pair";
  JsonDocument doc;
  doc["token"] = pairingToken;
  doc["ts"] = millis();
  
  // Serialize and publish
  char buffer[128];
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer);
  Serial.println("[Pair] Token sent: " + pairingToken);
}