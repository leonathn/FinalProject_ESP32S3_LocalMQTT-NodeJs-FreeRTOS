/**
 * @file config.h
 * @brief Hardware pin definitions and system configuration constants
 * 
 * This file centralizes all hardware pin assignments and system-wide configuration
 * constants. Modify these values to adapt the firmware to different hardware setups.
 * 
 * Hardware Target: ESP32-S3 DevKit
 * Board: esp32-s3-devkitc-1
 * Purpose: 8-Channel GPIO Actuator Device
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== GPIO PIN DEFINITIONS ==========
// 8-Channel GPIO pin assignments for relay/LED control
// Based on project_report.tex GPIO Channel Assignments
#define GPIO_CH1 13            // Channel 1 (Relay/LED)
#define GPIO_CH2 12            // Channel 2 (Relay/LED)
#define GPIO_CH3 14            // Channel 3 (Relay/LED)
#define GPIO_CH4 27            // Channel 4 (Relay/LED)
#define GPIO_CH5 26            // Channel 5 (Relay/LED)
#define GPIO_CH6 25            // Channel 6 (Relay/LED)
#define GPIO_CH7 33            // Channel 7 (Relay/LED)
#define GPIO_CH8 32            // Channel 8 (Relay/LED)

// GPIO array for easy iteration
const int GPIO_PINS[8] = {GPIO_CH1, GPIO_CH2, GPIO_CH3, GPIO_CH4, GPIO_CH5, GPIO_CH6, GPIO_CH7, GPIO_CH8};

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
#define STATUS_PUBLISH_INTERVAL_MS 20000   // Heartbeat/status publishing interval (20 seconds)
#define UI_UPDATE_INTERVAL_MS 500          // NeoPixel LED update frequency
#define MQTT_LOOP_INTERVAL_MS 100          // MQTT client loop processing frequency

// ========== BUTTON CONFIGURATION ==========
// Long press detection for configuration reset
#define BUTTON_LONG_PRESS_MS 3000       // Duration to hold button for factory reset

#endif // CONFIG_H
