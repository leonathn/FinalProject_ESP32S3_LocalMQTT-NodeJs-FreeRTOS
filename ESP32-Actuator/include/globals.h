/*
 * Global Variables and Objects - Actuator Version
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ESPmDNS.h>
#include "config.h"
#include "types.h"

// ========== OBJECTS ==========
extern Preferences prefs;
extern WebServer webServer;
extern DNSServer dnsServer;
extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern Adafruit_NeoPixel pixel;

// ========== CONFIGURATION VARIABLES ==========
extern String deviceId;
extern String wifiSSID;
extern String wifiPassword;
extern String mqttServer;
extern int mqttPort;
extern String pairingToken;

// ========== STATE VARIABLES ==========
extern bool apMode;
extern bool wifiConnected;
extern bool mqttConnected;

// ========== GPIO PINS AND STATES ==========
extern const uint8_t gpioOutputPins[8];
extern bool gpioStates[8];

// ========== FREERTOS HANDLES ==========
extern SemaphoreHandle_t commandMutex;
extern QueueHandle_t commandQueue;
extern EventGroupHandle_t connectionEvents;

#endif // GLOBALS_H
