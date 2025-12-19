/*
 * NeoPixel Handler Implementation
 */

#include "neopixel_handler.h"
#include "globals.h"

void updateNeoPixel() {
  uint32_t color;
  
  if (apMode) {
    color = pixel.Color(255, 128, 0); // Orange = AP mode
  } else if (!wifiConnected) {
    color = pixel.Color(255, 0, 0); // Red = connecting
  } else if (!mqttConnected) {
    color = pixel.Color(0, 0, 255); // Blue = WiFi only
  } else {
    color = pixel.Color(0, 255, 0); // Green = all good
  }
  
  pixel.setPixelColor(0, color);
  pixel.show();
}
