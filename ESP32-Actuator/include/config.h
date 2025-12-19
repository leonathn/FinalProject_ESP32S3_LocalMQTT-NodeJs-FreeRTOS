/*
 * Configuration and Pin Definitions - Actuator Version
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== PIN DEFINITIONS ==========
#define NEOPIXEL_WIFI 45
#define RESET_BUTTON_PIN 0  // Boot button on ESP32-S3 DevKit
#define RELAY_PIN 13        // Example relay pin
#define LED_PIN 12          // Example LED pin

// ========== CONFIGURATION ==========
#define AP_SSID_PREFIX "ESP32-IOT-ACTUATOR-"
#define AP_PASSWORD "12345678"
#define DNS_PORT 53
#define WEB_PORT 80

// ========== FREERTOS EVENT BITS ==========
#define WIFI_CONNECTED_BIT (1 << 0)
#define MQTT_CONNECTED_BIT (1 << 1)

// ========== TASK INTERVALS ==========
#define UI_UPDATE_INTERVAL_MS 500
#define MQTT_LOOP_INTERVAL_MS 100

// ========== BUTTON CONFIG ==========
#define CONFIG_RESET_HOLD_MS 3000  // Hold for 3 seconds to reset config

#endif // CONFIG_H
