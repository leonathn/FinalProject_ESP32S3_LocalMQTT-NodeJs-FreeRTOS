/**
 * @file wifi_manager.h
 * @brief WiFi connection management (AP and Station modes)
 * 
 * Handles WiFi connectivity in two modes:
 * 1. AP Mode: Creates WiFi hotspot for device configuration
 * 2. STA Mode: Connects to existing WiFi network
 * 
 * Functions manage mode switching, network scanning, and connection establishment.
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

/**
 * @brief Start WiFi Access Point mode for configuration
 * 
 * Creates a WiFi hotspot with SSID based on device ID.
 * Enables captive portal DNS server for automatic redirect.
 * Updates NeoPixel LED to orange to indicate AP mode.
 */
void startAPMode();

/**
 * @brief Start WiFi Station mode
 * 
 * Switches WiFi to client mode and initiates connection to configured network.
 * Calls connectWiFi() to perform actual connection.
 */
void startSTAMode();

/**
 * @brief Connect to WiFi network using stored credentials
 * 
 * Performs network scan, attempts connection with retry logic.
 * On success: Sets event bits, initiates MQTT connection.
 * On failure: Falls back to AP mode.
 */
void connectWiFi();

#endif // WIFI_MANAGER_H
