/*
 * Configuration Management Implementation
 */

#include "config_manager.h"
#include "globals.h"
#include <Arduino.h>

void loadConfig() {
  prefs.begin("esp32-iot", false);
  wifiSSID = prefs.getString("ssid", "");
  wifiPassword = prefs.getString("password", "");
  mqttServer = prefs.getString("mqtt_server", "");
  mqttPort = prefs.getInt("mqtt_port", 1883);
  pairingToken = prefs.getString("token", String(random(100000, 999999)));
  prefs.end();
}

void saveConfig() {
  prefs.begin("esp32-iot", false);
  prefs.putString("ssid", wifiSSID);
  prefs.putString("password", wifiPassword);
  prefs.putString("mqtt_server", mqttServer);
  prefs.putInt("mqtt_port", mqttPort);
  prefs.putString("token", pairingToken);
  prefs.end();
  Serial.println("[Config] Saved to NVS");
}

void resetConfig() {
  prefs.begin("esp32-iot", false);
  prefs.clear();
  prefs.end();
  Serial.println("[Config] ⚠ All settings erased!");
}
