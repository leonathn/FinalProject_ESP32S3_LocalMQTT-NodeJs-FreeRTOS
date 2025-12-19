/**
 * @file config_manager.h
 * @brief Non-Volatile Storage (NVS) configuration management
 * 
 * Handles persistent storage of device configuration in ESP32 flash memory.
 * Configuration survives reboots and power cycles.
 * 
 * Stored Settings:
 * - WiFi SSID and password
 * - MQTT broker address and port
 * - Device pairing token
 * 
 * Storage namespace: "esp32-iot"
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

/**
 * @brief Load configuration from NVS flash memory
 * 
 * Reads WiFi credentials, MQTT settings, and pairing token from NVS.
 * Generates new random pairing token if none exists.
 * Called during setup() before WiFi initialization.
 */
void loadConfig();

/**
 * @brief Save current configuration to NVS flash memory
 * 
 * Writes all configuration variables to persistent storage.
 * Called before rebooting when user saves settings via web interface.
 */
void saveConfig();

#endif // CONFIG_MANAGER_H
