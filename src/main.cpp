/*
 * ESP32-S3 IoT Device with WiFi Manager + FreeRTOS + Diagnostics
 * 
 * Hardware:
 * - I2C: SDA=11, SCL=12
 * - DHT20 sensor @ 0x38
 * - NeoPixel WiFi LED @ GPIO 45
 * 
 * Features:
 * - AP mode WiFi config captive portal
 * - FreeRTOS tasks with semaphore synchronization
 * - MQTT with pairing token
 * - Comprehensive diagnostics
 * 
 * Modular Structure:
 * - config.h: Pin definitions and constants
 * - types.h: Data structures
 * - globals.h: Global variables and objects
 * - config_manager: NVS storage
 * - wifi_manager: WiFi AP/STA mode
 * - mqtt_handler: MQTT connection and messaging
 * - neopixel_handler: LED status indicators
 * - diagnostics: System health checks
 * - web_server: Captive portal and API
 * - tasks: FreeRTOS task implementations
 */

#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "types.h"
#include "globals.h"
#include "config_manager.h"
#include "wifi_manager.h"
#include "mqtt_handler.h"
#include "neopixel_handler.h"
#include "diagnostics.h"
#include "web_server.h"
#include "tasks.h"

// ========== GLOBAL OBJECT INSTANCES ==========
Preferences prefs;
WebServer webServer(WEB_PORT);
DNSServer dnsServer;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
Adafruit_NeoPixel pixel(1, NEOPIXEL_WIFI, NEO_GRB + NEO_KHZ800);
DHT20 dht20;

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

SemaphoreHandle_t i2cMutex;
QueueHandle_t telemetryQueue;
QueueHandle_t sensorEventQueue;  // For sensor events/errors
EventGroupHandle_t connectionEvents;

DiagnosticsData lastDiagnostics;
SensorState sensorState = {TempBand::NORMAL, HumBand::COMFORT, 0, 0, 0, false};

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
    configResetHandled = false;
    Serial.println("[Button] Pressed - hold for 3 seconds to reset config");
  } 
  else if (!currentState && buttonPressed) {
    // Button released
    unsigned long pressDuration = millis() - buttonPressStart;
    buttonPressed = false;
    
    if (pressDuration < BUTTON_LONG_PRESS_MS) {
      Serial.println("[Button] Released (short press - ignored)");
    }
  }
  else if (currentState && buttonPressed && !configResetHandled) {
    // Button still pressed, check duration
    unsigned long pressDuration = millis() - buttonPressStart;
    
    if (pressDuration >= BUTTON_LONG_PRESS_MS) {
      configResetHandled = true;
      Serial.println("\n" + String('=', 50));
      Serial.println("[Button] LONG PRESS DETECTED - RESETTING CONFIG!");
      Serial.println(String('=', 50));
      
      // Blink LED rapidly to indicate reset
      for (int i = 0; i < 10; i++) {
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
        pixel.show();
        delay(100);
        pixel.setPixelColor(0, pixel.Color(0, 0, 0));
        pixel.show();
        delay(100);
      }
      
      // Clear saved configuration
      prefs.begin("esp32-iot", false);
      prefs.clear();
      prefs.end();
      
      Serial.println("[Config] Cleared! Rebooting to AP mode...");
      delay(1000);
      ESP.restart();
    }
  }
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n" + String('=', 50));
  Serial.println("ESP32-S3 IoT Device v2.0 (Modular)");
  Serial.println(String('=', 50));
  
  // Setup reset button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  Serial.println("[Button] Reset button on GPIO " + String(RESET_BUTTON_PIN) + " (hold 3s to reset config)");
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000); // 100kHz for DHT20 stability
  Serial.println("[I2C] Initialized on SDA=" + String(SDA_PIN) + ", SCL=" + String(SCL_PIN) + " @ 100kHz");
  delay(100); // Let I2C stabilize
  
  // Initialize NeoPixel
  pixel.begin();
  pixel.setBrightness(50);
  pixel.setPixelColor(0, pixel.Color(255, 128, 0)); // Orange = booting
  pixel.show();
  
  // Initialize DHT20 sensor
  Serial.println("[DHT20] Starting initialization...");
  if (dht20.begin()) {
    Serial.println("[DHT20] ✓ Sensor initialized successfully!");
    // First read to wake up sensor
    delay(100);
    int status = dht20.read();
    if (status == 0) {
      Serial.printf("[DHT20] ✓ Wake-up read OK (T:%.1f°C, H:%.1f%%)\n", 
                    dht20.getTemperature(), dht20.getHumidity());
    } else {
      Serial.printf("[DHT20] ⚠ Wake-up read failed (status: %d)\n", status);
    }
  } else {
    Serial.println("[DHT20] ✗ WARNING: Sensor initialization failed!");
  }
  delay(100);
  
  // Scan I2C bus for debugging
  Serial.println("[I2C] Scanning bus...");
  byte count = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("[I2C] Found device at 0x%02X\n", addr);
      count++;
    }
  }
  if (count == 0) {
    Serial.println("[I2C] ✗ No devices found! Check wiring and pull-up resistors.");
  } else {
    Serial.printf("[I2C] ✓ Found %d device(s)\n", count);
  }
  
  // Re-initialize DHT20 after scan
  if (dht20.begin()) {
    Serial.println("[DHT20] Initialized");
  } else {
    Serial.println("[DHT20] Init failed!");
  }
  
  // Generate device ID from MAC
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId = "ESP32-IOT-SENSORS-" + String(mac[4], HEX) + String(mac[5], HEX);
  Serial.println("[Device] ID: " + deviceId);
  
  // Load config
  loadConfig();
  
  // Create FreeRTOS synchronization primitives
  i2cMutex = xSemaphoreCreateMutex();
  telemetryQueue = xQueueCreate(10, sizeof(TelemetryData));  // Buffer 10 readings
  sensorEventQueue = xQueueCreate(20, sizeof(SensorEvent));  // Buffer 20 events
  connectionEvents = xEventGroupCreate();
  
  if (!i2cMutex || !telemetryQueue || !sensorEventQueue || !connectionEvents) {
    Serial.println("[FreeRTOS] Failed to create primitives!");
    while (1) delay(1000);
  }
  Serial.println("[FreeRTOS] Mutex, Queues (T:10, E:20), and Events created");
  
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
  
  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(TaskSensors, "Sensors", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskUI, "UI", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskMQTT, "MQTT", 4096, NULL, 2, NULL, 1);
  
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
