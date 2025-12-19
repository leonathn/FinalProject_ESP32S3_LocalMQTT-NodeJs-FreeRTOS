/**
 * @file globals.h
 * @brief Global variable declarations and extern object references
 * 
 * This file declares all global variables and external object instances that are
 * shared across multiple translation units. The actual definitions are in main.cpp.
 * 
 * Architecture:
 * - Objects: Hardware peripherals and library instances
 * - Variables: Configuration settings and state flags  
 * - FreeRTOS: Synchronization primitives (event groups)
 * 
 * All variables declared here are defined/instantiated in main.cpp setup().
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "types.h"

// ========== HARDWARE AND LIBRARY OBJECTS ==========
// These objects are instantiated in main.cpp and used across multiple modules

extern Preferences prefs;              // NVS (Non-Volatile Storage) for persistent config
extern WebServer webServer;            // HTTP server for configuration portal
extern DNSServer dnsServer;            // DNS server for captive portal redirection
extern WiFiClient espClient;           // TCP client for MQTT connection
extern PubSubClient mqttClient;        // MQTT client for command subscription
extern Adafruit_NeoPixel pixel;        // WS2812B RGB LED controller

// ========== CONFIGURATION VARIABLES ==========
// Persistent configuration loaded from NVS on boot
// Modified via web interface, saved back to NVS before reboot

extern String deviceId;                // Unique device identifier (MAC-based)
extern String wifiSSID;                // WiFi network name (SSID)
extern String wifiPassword;            // WiFi network password
extern String mqttServer;              // MQTT broker IP address or hostname
extern int mqttPort;                   // MQTT broker port (default: 1883)

// ========== STATE VARIABLES ==========
// Runtime state flags updated by tasks
// Used for coordinating behavior across modules

extern bool apMode;                    // True if in AP (Access Point) mode
extern bool wifiConnected;             // True if connected to WiFi station
extern bool mqttConnected;             // True if connected to MQTT broker

// ========== GPIO STATE ==========
// Current state of all 8 GPIO channels
extern GPIOState currentGPIOState;

// ========== FREERTOS SYNCHRONIZATION PRIMITIVES ==========
// Handles for inter-task communication and synchronization
// Created in main.cpp setup(), used by all tasks

extern EventGroupHandle_t connectionEvents; // Event bits for WiFi/MQTT connection states

#endif // GLOBALS_H
