/**
 * @file neopixel_handler.h
 * @brief NeoPixel RGB LED status indicator control
 * 
 * Controls WS2812B RGB LED to provide visual feedback of device state.
 * 
 * LED Color States:
 * - Orange: AP mode (configuration needed)
 * - Red: Connecting to WiFi
 * - Blue: WiFi connected, MQTT disconnected  
 * - Green: Fully operational (WiFi + MQTT connected)
 */

#ifndef NEOPIXEL_HANDLER_H
#define NEOPIXEL_HANDLER_H

/**
 * @brief Update NeoPixel LED color based on system state
 * 
 * Reads global connection state flags (apMode, wifiConnected, mqttConnected)
 * and sets LED color accordingly. Called by TaskUI every 500ms.
 */
void updateNeoPixel();

#endif // NEOPIXEL_HANDLER_H
