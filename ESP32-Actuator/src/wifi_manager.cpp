/**
 * @file wifi_manager.cpp
 * @brief WiFi Management Implementation
 * 
 * Handles WiFi connectivity in both Access Point (AP) and Station (STA) modes.
 * Provides network scanning, connection management, and automatic fallback to AP mode.
 */

#include "wifi_manager.h"
#include "globals.h"
#include "neopixel_handler.h"
#include "mqtt_handler.h"
#include <Arduino.h>

/**
 * @brief Start WiFi Access Point mode for device configuration
 * 
 * Creates a WiFi hotspot with SSID based on device ID and fixed password.
 * Starts DNS server for captive portal to redirect all requests to web interface.
 * Updates global state and LED indicator.
 */
void startAPMode() {
  apMode = true;  // Set global flag for AP mode
  String apSSID = deviceId;  // Use device ID as hotspot name
  
  // Configure and start WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID.c_str(), AP_PASSWORD);
  
  // Log AP details
  Serial.println("[AP] Started: " + apSSID);
  Serial.println("[AP] Password: " + String(AP_PASSWORD));
  Serial.println("[AP] IP: " + WiFi.softAPIP().toString());
  
  // Start captive portal DNS server
  // Redirects all DNS requests to AP IP (192.168.4.1)
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  // Update LED to show AP mode (orange)
  updateNeoPixel();
}

/**
 * @brief Start WiFi Station mode
 * 
 * Switches device to WiFi client mode and initiates connection
 * to the configured network.
 */
void startSTAMode() {
  apMode = false;  // Clear AP mode flag
  WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  connectWiFi();  // Attempt connection
}

/**
 * @brief Connect to WiFi network using stored credentials
 * 
 * Performs comprehensive network connection:
 * 1. Scans for available networks (helps debug connectivity issues)
 * 2. Attempts connection with timeout and retry logic
 * 3. On success: Sets event bits, initiates MQTT
 * 4. On failure: Falls back to AP mode for reconfiguration
 */
void connectWiFi() {
  // Skip if no WiFi credentials configured
  if (wifiSSID.length() == 0) return;
  
  // ===== STEP 1: Scan for available networks =====
  // Helps debug connectivity issues by showing what's visible
  Serial.println("[WiFi] Scanning networks...");
  int networksFound = WiFi.scanNetworks();
  Serial.println("[WiFi] Found " + String(networksFound) + " networks:");
  
  bool targetFound = false;
  for (int i = 0; i < networksFound; i++) {
    Serial.println("  " + String(i + 1) + ". " + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)");
    
    // Check if target network is visible
    if (WiFi.SSID(i) == wifiSSID) {
      targetFound = true;
      Serial.println("     ✓ Target network found!");
    }
  }
  
  // Warn if target network not found (may still connect if hidden)
  if (!targetFound) {
    Serial.println("[WiFi] Warning: Target network '" + wifiSSID + "' not found in scan");
  }
  
  // ===== STEP 2: Attempt connection =====
  Serial.print("[WiFi] Connecting to: " + wifiSSID);
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  
  // Wait for connection with timeout (20 seconds)
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
    updateNeoPixel();  // Keep LED updated during connection
  }
  
  // ===== STEP 3: Handle connection result =====
  if (WiFi.status() == WL_CONNECTED) {
    // ===== CONNECTION SUCCESS =====
    wifiConnected = true;
    xEventGroupSetBits(connectionEvents, WIFI_CONNECTED_BIT);  // Notify other tasks
    
    Serial.println("\n[WiFi] Connected!");
    Serial.println("[WiFi] IP: " + WiFi.localIP().toString());
    Serial.println("[WiFi] RSSI: " + String(WiFi.RSSI()) + " dBm");
    
    updateNeoPixel();  // Update LED to show WiFi connected
    
    // Connect to MQTT broker if configured
    if (mqttServer.length() > 0) {
      connectMQTT();
    }
  } else {
    // ===== CONNECTION FAILED =====
    Serial.println("\n[WiFi] Failed! Switching to AP mode");
    startAPMode();  // Fall back to configuration mode
  }
}
