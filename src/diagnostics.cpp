/**
 * @file diagnostics.cpp
 * @brief System Diagnostics Implementation
 * 
 * Performs comprehensive hardware and sensor diagnostics:
 * - I2C bus scan for connected devices
 * - DHT20 sensor validation and test reads
 * - NeoPixel LED functional test
 * 
 * Results are stored globally and published via MQTT for remote monitoring.
 */

#include "diagnostics.h"
#include "globals.h"
#include "neopixel_handler.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

/**
 * @brief Run full system diagnostics
 * 
 * Comprehensive system health check:
 * 1. I2C bus scan - Detects all devices on I2C bus
 * 2. DHT20 validation - Re-initializes sensor and performs test read
 * 3. NeoPixel test - Brief LED flash test
 * 4. Results publishing - Sends diagnostics via MQTT
 * 
 * Thread-safe: Acquires i2cMutex before I2C operations.
 * Results stored in global 'lastDiagnostics' structure.
 */
void runDiagnostics() {
  Serial.println("[Diag] Running diagnostics...");
  
  // ===== ACQUIRE I2C MUTEX =====
  // Prevent sensor task from accessing I2C during diagnostics
  if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
    
    // ===== I2C BUS SCAN =====
    // Scan all possible I2C addresses (1-126) for devices
    lastDiagnostics.i2cDevices = "";
    int deviceCount = 0;
    for (byte addr = 1; addr < 127; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {  // Device ACKed
        if (deviceCount > 0) lastDiagnostics.i2cDevices += ", ";
        lastDiagnostics.i2cDevices += "0x" + String(addr, HEX);
        deviceCount++;
      }
    }
    lastDiagnostics.i2cOk = (deviceCount > 0);
    
    // ===== DHT20 SENSOR VALIDATION =====
    lastDiagnostics.dht20Ok = false;
    Serial.println("[Diag] Testing DHT20...");
    
    // Re-initialize DHT20 for clean test
    if (dht20.begin()) {
      delay(100);  // Sensor stabilization time
      
      // First read to wake up sensor (may return stale data)
      dht20.read();
      delay(100);
      
      // Second read for actual data
      int status = dht20.read();
      if (status == 0) {  // Read successful
        float temp = dht20.getTemperature();
        float hum = dht20.getHumidity();
        Serial.printf("[Diag] DHT20 read: T=%.1f°C, H=%.1f%%, status=%d\n", temp, hum, status);
        
        // Validate reading ranges
        if (temp > -10 && temp < 60 && hum >= 0 && hum <= 100) {
          lastDiagnostics.dht20Ok = true;
          lastDiagnostics.temperature = temp;
          lastDiagnostics.humidity = hum;
          Serial.println("[Diag] DHT20 OK!");
        } else {
          Serial.println("[Diag] DHT20 values out of range");
        }
      } else {
        Serial.printf("[Diag] DHT20 read failed with status: %d\n", status);
      }
    } else {
      Serial.println("[Diag] DHT20 initialization failed");
    }
    
    // ===== RELEASE I2C MUTEX =====
    xSemaphoreGive(i2cMutex);
  }
  
  // ===== NEOPIXEL LED TEST =====
  // Brief flash to red, then back to normal state
  lastDiagnostics.neopixelOk = true;
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));  // Red flash
  pixel.show();
  delay(100);
  updateNeoPixel();  // Restore normal state indicator
  
  Serial.println("[Diag] Complete");
  
  // ===== PUBLISH DIAGNOSTICS VIA MQTT =====
  if (mqttConnected) {
    String topic = "devices/" + deviceId + "/diagnostics";
    JsonDocument doc;
    doc["i2c"] = lastDiagnostics.i2cDevices;
    doc["dht20"]["ok"] = lastDiagnostics.dht20Ok;
    doc["dht20"]["tC"] = lastDiagnostics.temperature;
    doc["dht20"]["rh"] = lastDiagnostics.humidity;
    doc["neopixel"]["ok"] = lastDiagnostics.neopixelOk;
    doc["ts"] = millis();
    
    char buffer[512];
    serializeJson(doc, buffer);
    mqttClient.publish(topic.c_str(), buffer);
  }
}
