/*
 * WiFi Management Implementation
 */

#include "wifi_manager.h"
#include "globals.h"
#include "neopixel_handler.h"
#include "mqtt_handler.h"
#include <Arduino.h>

void startAPMode() {
  apMode = true;
  String apSSID = AP_SSID_PREFIX + deviceId;
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID.c_str(), AP_PASSWORD);
  
  Serial.println("[AP] Started: " + apSSID);
  Serial.println("[AP] Password: " + String(AP_PASSWORD));
  Serial.println("[AP] IP: " + WiFi.softAPIP().toString());
  
  // Captive portal
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  updateNeoPixel();
}

void startSTAMode() {
  apMode = false;
  WiFi.mode(WIFI_STA);
  connectWiFi();
}

void connectWiFi() {
  if (wifiSSID.length() == 0) return;
  
  // Scan for WiFi networks
  Serial.println("[WiFi] Scanning networks...");
  int networksFound = WiFi.scanNetworks();
  Serial.println("[WiFi] Found " + String(networksFound) + " networks:");
  
  bool targetFound = false;
  for (int i = 0; i < networksFound; i++) {
    Serial.println("  " + String(i + 1) + ". " + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)");
    if (WiFi.SSID(i) == wifiSSID) {
      targetFound = true;
      Serial.println("     ✓ Target network found!");
    }
  }
  
  if (!targetFound) {
    Serial.println("[WiFi] Warning: Target network '" + wifiSSID + "' not found in scan");
  }
  
  Serial.print("[WiFi] Connecting to: " + wifiSSID);
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
    updateNeoPixel();
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    xEventGroupSetBits(connectionEvents, WIFI_CONNECTED_BIT);
    Serial.println("\n[WiFi] Connected!");
    Serial.println("[WiFi] IP: " + WiFi.localIP().toString());
    Serial.println("[WiFi] RSSI: " + String(WiFi.RSSI()) + " dBm");
    updateNeoPixel();
    
    // Start mDNS
    if (MDNS.begin(deviceId.c_str())) {
      Serial.println("[mDNS] Started as " + deviceId + ".local");
    } else {
      Serial.println("[mDNS] Failed to start");
    }
    
    // Connect MQTT
    if (mqttServer.length() > 0) {
      connectMQTT();
    }
  } else {
    Serial.println("\n[WiFi] Failed! Switching to AP mode");
    startAPMode();
  }
}
