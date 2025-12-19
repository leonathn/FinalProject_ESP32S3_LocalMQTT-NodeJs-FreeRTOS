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
 * 
 * Storage namespace: "esp32-actuator"
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

/**
 * @brief Load configuration from NVS flash memory
 * 
 * Reads WiFi credentials and MQTT settings from NVS.
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
