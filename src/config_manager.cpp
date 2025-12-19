/**
 * @file config_manager.cpp
 * @brief Configuration Management Implementation
 * 
 * Handles persistent storage of device configuration using ESP32 NVS (Non-Volatile Storage).
 * Configuration is stored in flash memory and survives reboots and power cycles.
 */

#include "config_manager.h"
#include "globals.h"
#include <Arduino.h>

/**
 * @brief Load configuration from NVS flash memory
 * 
 * Opens NVS namespace "esp32-iot" and reads all stored configuration values.
 * If a value doesn't exist (first boot), uses default values:
 * - Empty strings for WiFi credentials (triggers AP mode)
 * - Default MQTT port 1883
 * - Random 6-digit pairing token
 */
void loadConfig() {
  prefs.begin("esp32-iot", false);  // Open NVS namespace in read-write mode
  
  // Load WiFi credentials (empty on first boot)
  wifiSSID = prefs.getString("ssid", "");
  wifiPassword = prefs.getString("password", "");
  
  // Load MQTT broker settings
  mqttServer = prefs.getString("mqtt_server", "");
  mqttPort = prefs.getInt("mqtt_port", 1883);  // Default MQTT port
  
  // Load or generate pairing token (6-digit random number)
  pairingToken = prefs.getString("token", String(random(100000, 999999)));
  
  prefs.end();  // Close NVS namespace
}

/**
 * @brief Save current configuration to NVS flash memory
 * 
 * Writes all global configuration variables to persistent storage.
 * Called when user saves settings via web interface, before rebooting.
 */
void saveConfig() {
  prefs.begin("esp32-iot", false);  // Open NVS namespace in read-write mode
  
  // Save WiFi credentials
  prefs.putString("ssid", wifiSSID);
  prefs.putString("password", wifiPassword);
  
  // Save MQTT broker settings
  prefs.putString("mqtt_server", mqttServer);
  prefs.putInt("mqtt_port", mqttPort);
  
  // Save pairing token
  prefs.putString("token", pairingToken);
  
  prefs.end();  // Close NVS namespace
  
  Serial.println("[Config] Saved to NVS");
}
