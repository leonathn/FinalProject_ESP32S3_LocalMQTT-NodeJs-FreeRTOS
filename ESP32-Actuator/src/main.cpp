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

SemaphoreHandle_t commandMutex;
QueueHandle_t commandQueue;
EventGroupHandle_t connectionEvents;

// ========== ACTUATOR STATE ==========
// GPIO output pins mapping (8 channels)
const uint8_t gpioOutputPins[8] = {
  5, 6, 7, 8,      // GPIO 1-4: Channels 1-4
  9, 10, 21, 38    // GPIO 5-8: Channels 5-8
};

// GPIO state tracking
bool gpioStates[8] = {false, false, false, false, false, false, false, false};

struct ActuatorState {
  bool relayState;
  uint8_t ledBrightness;
  uint32_t ledColor;
  uint32_t commandCount;
  uint32_t errorCount;
} actuatorState = {false, 0, 0, 0, 0};

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
  commandQueue = xQueueCreate(20, sizeof(String));  // Buffer 20 commands
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
