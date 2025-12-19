/*
 * Actuator Main - MQTT Subscriber for controlling actuators
 */

#include "config.h"
#include "types.h"
#include "globals.h"
#include "config_manager.h"
#include "wifi_manager.h"
#include "mqtt_handler.h"
#include "web_server.h"
#include "neopixel_handler.h"
#include "tasks.h"
#include <Arduino.h>
#include <ArduinoJson.h>

// Define constants if not found in config.h
#ifndef CONFIG_RESET_HOLD_MS
#define CONFIG_RESET_HOLD_MS 3000
#endif

// Forward declarations
void resetConfig();  // from config_manager.cpp
void TaskActuator(void *pvParameters);
void TaskUI(void *pvParameters);
void TaskMQTT(void *pvParameters);

// Sensor stubs (actuator doesn't use sensors but web_server references them)
SemaphoreHandle_t i2cMutex = NULL;
DHT20 dht20;
SensorState sensorState;
DiagnosticsData lastDiagnostics;

void runDiagnostics() {
  // No-op for actuator
}

// ========== GLOBAL OBJECT INSTANCES ==========
Preferences prefs;
WebServer webServer(WEB_PORT);
DNSServer dnsServer;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
Adafruit_NeoPixel pixel(1, NEOPIXEL_WIFI, NEO_GRB + NEO_KHZ800);

// ========== GLOBAL VARIABLES ==========
String deviceId;
String wifiSSID;
String wifiPassword;
String mqttServer;
int mqttPort = 1883;
String pairingToken;

bool apMode = false;
bool wifiConnected = false;
bool mqttConnected = false;

SemaphoreHandle_t commandMutex = NULL;
QueueHandle_t commandQueue = NULL;
EventGroupHandle_t connectionEvents = NULL;

// ========== ACTUATOR STATE ==========
// GPIO output pins mapping (Matching board pinout: D2-D11 connectors)
const uint8_t gpioOutputPins[8] = {
  5, 6, 7, 8,      // GPIO 1-4: D2-D5 Relay outputs
  9, 10, 21, 38    // GPIO 5-8: D6-D11 Relay outputs
};

ActuatorState actuatorState = {  {false, false, false, false, false, false, false, false}, 0, 0, 0};

// ========== BUTTON STATE ==========
unsigned long buttonPressStart = 0;
bool buttonPressed = false;
bool configResetHandled = false;

// ========== BUTTON FUNCTIONS ==========
void checkResetButton() {
  bool currentState = digitalRead(RESET_BUTTON_PIN) == LOW; // Active LOW
  
  if (currentState && !buttonPressed) {
    // Button just pressed
    buttonPressed = true;
    buttonPressStart = millis();
    Serial.println("[Button] Reset button pressed");
  }
  else if (!currentState && buttonPressed) {
    // Button just released
    buttonPressed = false;
    configResetHandled = false;
    Serial.println("[Button] Reset button released");
  }
  else if (currentState && buttonPressed) {
    // Button held down - check duration
    unsigned long holdTime = millis() - buttonPressStart;
    
    if (holdTime >= CONFIG_RESET_HOLD_MS && !configResetHandled) {
      configResetHandled = true;
      Serial.println("[Button] ⚠ CONFIG RESET TRIGGERED!");
      
      // Visual feedback
      pixel.setPixelColor(0, pixel.Color(255, 0, 0));
      pixel.show();
      delay(500);
      pixel.setPixelColor(0, pixel.Color(0, 0, 0));
      pixel.show();
      delay(200);
      pixel.setPixelColor(0, pixel.Color(255, 0, 0));
      pixel.show();
      delay(500);
      
      // Reset config
      resetConfig();
      
      // Reboot
      Serial.println("[System] Rebooting...");
      delay(1000);
      ESP.restart();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n" + String('=', 50));
  Serial.println("ESP32-S3 Actuator Device v2.0");
  Serial.println(String('=', 50));
  
  // Setup reset button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  Serial.println("[Button] Reset button on GPIO " + String(RESET_BUTTON_PIN) + " (hold 3s to reset config)");
  
  // Initialize NeoPixel
  pixel.begin();
  pixel.setBrightness(50);
  pixel.setPixelColor(0, pixel.Color(255, 128, 0)); // Orange = booting
  pixel.show();
  
  // Generate device ID from MAC
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId = "ESP32-IOT-ACTUATOR-" + String(mac[4], HEX) + String(mac[5], HEX);
  Serial.println("[Device] ID: " + deviceId);
  
  // Load config
  loadConfig();
  
  // Create FreeRTOS synchronization primitives
  commandMutex = xSemaphoreCreateMutex();
  commandQueue = xQueueCreate(20, 256);  // Buffer 20 commands (256 bytes each)
  connectionEvents = xEventGroupCreate();
  
  if (!commandMutex || !commandQueue || !connectionEvents) {
    Serial.println("[FreeRTOS] Failed to create primitives!");
    while (1) delay(1000);
  }
  Serial.println("[FreeRTOS] Mutex, Queue (20), and Events created");
  
  // Determine mode
  if (wifiSSID.length() == 0) {
    Serial.println("[WiFi] No config found, starting AP mode");
    startAPMode();
  } else {
    Serial.println("[WiFi] Config found, connecting to: " + wifiSSID);
    startSTAMode();
  }
  
  // Setup web server
  setupWebServer();
  
  // Create FreeRTOS tasks (no sensor task)
  xTaskCreatePinnedToCore(TaskUI, "UI", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskMQTT, "MQTT", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskActuator, "Actuator", 4096, NULL, 2, NULL, 1);
  
  Serial.println("[Setup] Complete!");
}

// ========== MAIN LOOP ==========
void loop() {
  // Check reset button
  checkResetButton();
  
  if (apMode) {
    dnsServer.processNextRequest();
  }
  webServer.handleClient();
  delay(10);
}

void TaskActuator(void *pvParameters) {
  Serial.println("[Actuator] Task started");
  
  // Setup all GPIO output pins
  Serial.println("[GPIO] Initializing 8 output pins:");
  for (int i = 0; i < 8; i++) {
    pinMode(gpioOutputPins[i], OUTPUT);
    digitalWrite(gpioOutputPins[i], LOW);
    actuatorState.gpioStates[i] = false;
    Serial.println("  GPIO" + String(i + 1) + " -> Pin " + String(gpioOutputPins[i]));
  }
  
  Serial.println("[Actuator] Ready - Waiting for commands");
  
  char commandBuffer[256];
  for (;;) {
    // Check for commands in queue
    if (xQueueReceive(commandQueue, &commandBuffer, pdMS_TO_TICKS(100)) == pdTRUE) {
      String command = String(commandBuffer);
      Serial.println("[Actuator] Received command: " + command);
      
      // Parse JSON command
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, command);
      
      if (error) {
        Serial.println("[Actuator] Failed to parse command JSON");
        actuatorState.errorCount++;
        continue;
      }
      
      // Get command type (try both "cmd" and "type" fields for compatibility)
      const char* type = doc["type"] | doc["cmd"];
      
      if (strcmp(type, "gpio") == 0 || strcmp(type, "relay") == 0) {
        // GPIO ON/OFF command: {"cmd":"gpio", "pin":1, "state":true}
        int gpioNum = doc["pin"];  // 1-8
        bool state = doc["state"];
        
        if (gpioNum >= 1 && gpioNum <= 8) {
          int idx = gpioNum - 1;
          digitalWrite(gpioOutputPins[idx], state ? HIGH : LOW);
          actuatorState.gpioStates[idx] = state;
          Serial.println("[GPIO" + String(gpioNum) + "] Pin " + String(gpioOutputPins[idx]) + " -> " + String(state ? "ON" : "OFF"));
          actuatorState.commandCount++;
        } else {
          Serial.println("[Actuator] Invalid GPIO number: " + String(gpioNum));
          actuatorState.errorCount++;
        }
      }
      else if (strcmp(type, "neopixel") == 0 || strcmp(type, "led") == 0) {
        // NeoPixel color command: {"cmd":"neopixel", "color":{"r":255,"g":0,"b":0}}
        uint8_t r = doc["color"]["r"];
        uint8_t g = doc["color"]["g"];
        uint8_t b = doc["color"]["b"];
        pixel.setPixelColor(0, pixel.Color(r, g, b));
        pixel.show();
        actuatorState.ledColor = (r << 16) | (g << 8) | b;
        Serial.println("[NeoPixel] Color updated: R=" + String(r) + " G=" + String(g) + " B=" + String(b));
        actuatorState.commandCount++;
      }
      else {
        Serial.println("[Actuator] Unknown command type: " + String(type));
        actuatorState.errorCount++;
      }
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void TaskUI(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(500);  // Update every 500ms
  
  for (;;) {
    updateNeoPixel();
    vTaskDelay(xDelay);
  }
}

void TaskMQTT(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(100);
  
  for (;;) {
    // Wait for MQTT connection
    EventBits_t bits = xEventGroupWaitBits(
      connectionEvents,
      MQTT_CONNECTED_BIT,
      pdFALSE,
      pdTRUE,
      xDelay
    );
    
    if (bits & MQTT_CONNECTED_BIT) {
      mqttClient.loop();
      
      // Reconnect if needed
      if (!mqttClient.connected()) {
        xEventGroupClearBits(connectionEvents, MQTT_CONNECTED_BIT);
        mqttConnected = false;
        updateNeoPixel();
        
        connectMQTT();
      }
    } else {
      // Not connected, try to connect if WiFi is up
      EventBits_t wifiBits = xEventGroupGetBits(connectionEvents);
      if (wifiBits & WIFI_CONNECTED_BIT) {
        if (mqttServer.length() > 0) {
          connectMQTT();
        }
      }
    }
    
    vTaskDelay(xDelay);
  }
}