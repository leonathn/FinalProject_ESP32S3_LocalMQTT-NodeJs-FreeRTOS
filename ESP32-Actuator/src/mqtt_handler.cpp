/*
 * MQTT Handler Implementation
 */

#include "mqtt_handler.h"
#include "globals.h"
#include "neopixel_handler.h"
#include <Arduino.h>
#include <ArduinoJson.h>

void connectMQTT() {
  if (!wifiConnected || mqttServer.length() == 0) return;
  
  mqttClient.setServer(mqttServer.c_str(), mqttPort);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setBufferSize(512);
  
  Serial.print("[MQTT] Connecting to: " + mqttServer + ":" + String(mqttPort));
  
  if (mqttClient.connect(deviceId.c_str())) {
    mqttConnected = true;
    xEventGroupSetBits(connectionEvents, MQTT_CONNECTED_BIT);
    Serial.println(" Connected!");
    
    // Subscribe to GPIO control topic
    String gpioTopic = "device/" + deviceId + "/gpio/set";
    String cmdTopic = "device/" + deviceId + "/cmd";
    mqttClient.subscribe(gpioTopic.c_str());
    mqttClient.subscribe(cmdTopic.c_str());
    Serial.println("[MQTT] Subscribed to: " + gpioTopic);
    Serial.println("[MQTT] Subscribed to: " + cmdTopic);
    
    // Publish status
    publishStatus();
    updateNeoPixel();
  } else {
    Serial.println(" Failed, rc=" + String(mqttClient.state()));
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  
  Serial.println("[MQTT] " + topicStr + ": " + payloadStr);
  
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payloadStr);
  if (error) {
    Serial.println("[MQTT] JSON parse error");
    return;
  }
  
  // Handle GPIO control commands
  if (topicStr.endsWith("/gpio/set")) {
    // Forward GPIO command to actuator task
    if (commandQueue != NULL) {
      String command = payloadStr;
      xQueueSend(commandQueue, &command, pdMS_TO_TICKS(100));
      Serial.println("[MQTT] GPIO command queued");
    }
  }
  // Handle general commands
  else if (topicStr.endsWith("/cmd")) {
    const char* cmd = doc["cmd"];
    if (strcmp(cmd, "reboot") == 0) {
      Serial.println("[CMD] Rebooting...");
      delay(1000);
      ESP.restart();
    }
  }
}

void publishStatus() {
  if (!mqttConnected) return;
  
  String topic = "devices/" + deviceId + "/status";
  JsonDocument doc;
  doc["online"] = true;
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  doc["wifiMode"] = apMode ? "AP" : "STA";
  doc["ts"] = millis();
  
  char buffer[256];
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer, true);
}

void publishTelemetry() {
  if (!mqttConnected) return;
  
  String topic = "devices/" + deviceId + "/telemetry";
  JsonDocument doc;
  
  // Publish GPIO states
  JsonArray gpios = doc["gpio"].to<JsonArray>();
  for (int i = 0; i < 8; i++) {
    JsonObject gpio = gpios.add<JsonObject>();
    gpio["pin"] = i + 1;
    gpio["state"] = gpioStates[i];
    gpio["physicalPin"] = gpioOutputPins[i];
  }
  
  doc["uptime"] = millis() / 1000;
  doc["heap"] = ESP.getFreeHeap();
  doc["rssi"] = WiFi.RSSI();
  doc["ts"] = millis();
  
  char buffer[512];
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer);
  Serial.println("[MQTT] Telemetry published");
}
