/**
 * @file neopixel_handler.cpp
 * @brief NeoPixel LED Status Indicator Implementation
 * 
 * Controls WS2812B RGB LED to provide visual feedback of device state.
 * LED color indicates current operating mode and connection status.
 */

#include "neopixel_handler.h"
#include "globals.h"

/**
 * @brief Update NeoPixel LED color based on system connection state
 * 
 * Selects LED color according to priority:
 * 1. AP Mode (highest priority) - Orange
 * 2. WiFi Disconnected - Red
 * 3. MQTT Disconnected - Blue
 * 4. Fully Connected - Green
 * 
 * Called by TaskUI every 500ms to keep LED updated.
 */
void updateNeoPixel() {
  uint32_t color;
  
  // Determine color based on connection state (priority order)
  if (apMode) {
    // Orange = AP/Configuration mode active
    color = pixel.Color(255, 128, 0);
  } else if (!wifiConnected) {
    // Red = Attempting WiFi connection
    color = pixel.Color(255, 0, 0);
  } else if (!mqttConnected) {
    // Blue = WiFi connected, MQTT disconnected
    color = pixel.Color(0, 0, 255);
  } else {
    // Green = All systems operational (WiFi + MQTT)
    color = pixel.Color(0, 255, 0);
  }
  
  // Update LED hardware
  pixel.setPixelColor(0, color);  // Set color of first pixel (index 0)
  pixel.show();                    // Commit changes to LED
}
