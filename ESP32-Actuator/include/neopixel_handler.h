/**
 * @file neopixel_handler.h
 * @brief WS2812B NeoPixel LED control for status indication
 * 
 * Provides visual feedback of system state through RGB LED colors:
 * - Orange: Access Point mode (waiting for configuration)
 * - Red: Connecting to WiFi
 * - Blue: WiFi connected, connecting to MQTT
 * - Green: Fully operational (WiFi and MQTT connected)
 */

#ifndef NEOPIXEL_HANDLER_H
#define NEOPIXEL_HANDLER_H

/**
 * @brief Update NeoPixel LED color based on connection state
 * 
 * Reads global connection flags (apMode, wifiConnected, mqttConnected)
 * and sets LED color accordingly. Call after any connection state change.
 */
void updateNeoPixel();

#endif // NEOPIXEL_HANDLER_H
