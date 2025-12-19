/**
 * @file config.h
 * @brief Hardware pin definitions and system configuration constants
 * 
 * This file centralizes all hardware pin assignments and system-wide configuration
 * constants. Modify these values to adapt the firmware to different hardware setups.
 * 
 * Hardware Target: ESP32-S3 DevKit
 * Board: esp32-s3-devkitc-1
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== I2C PIN DEFINITIONS ==========
// I2C bus configuration for sensor communication
// Standard ESP32-S3 I2C pins, can be reassigned to any GPIO
#define SDA_PIN 11              // I2C Data line (Serial Data)
#define SCL_PIN 12              // I2C Clock line (Serial Clock)
#define DHT20_ADDR 0x38         // DHT20 sensor I2C address (fixed by manufacturer)

// ========== LED PIN DEFINITIONS ==========
#define NEOPIXEL_WIFI 45        // WS2812B RGB LED for WiFi/MQTT status indication

// ========== BUTTON PIN DEFINITIONS ==========
#define RESET_BUTTON_PIN 0      // Boot button on ESP32-S3 DevKit (active LOW)

// ========== WIFI ACCESS POINT CONFIGURATION ==========
// Default password for AP mode WiFi hotspot
// AP SSID is auto-generated from device MAC address
#define AP_PASSWORD "12345678"  // WPA2 password for configuration mode

// ========== NETWORK PORT CONFIGURATION ==========
#define DNS_PORT 53             // DNS server port for captive portal redirection
#define WEB_PORT 80             // HTTP web server port for configuration interface

// ========== FREERTOS EVENT BITS ==========
// Event group bits for tracking connection states across tasks
#define WIFI_CONNECTED_BIT (1 << 0)  // Bit 0: WiFi station connected
#define MQTT_CONNECTED_BIT (1 << 1)  // Bit 1: MQTT broker connected

// ========== TASK TIMING INTERVALS ==========
// Control loop frequencies for FreeRTOS tasks
#define SENSOR_READ_INTERVAL_MS 5000    // DHT20 sensor reading interval (unused, tasks uses 1000ms)
#define UI_UPDATE_INTERVAL_MS 500       // NeoPixel LED update frequency
#define MQTT_LOOP_INTERVAL_MS 100       // MQTT client loop processing frequency

// ========== BUTTON CONFIGURATION ==========
// Long press detection for configuration reset
#define BUTTON_LONG_PRESS_MS 3000       // Duration to hold button for factory reset

#endif // CONFIG_H
