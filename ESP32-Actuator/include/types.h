/*
 * Data Types and Structures - Actuator Version
 */

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// ========== ACTUATOR COMMAND ==========
struct ActuatorCommand {
  enum Type { RELAY, LED, NEOPIXEL, PWM, SERVO } type;
  uint8_t pin;
  bool state;
  uint8_t value;     // For PWM or brightness
  uint32_t color;    // For NeoPixel
  uint32_t timestamp;
};

// ========== ACTUATOR STATUS ==========
struct ActuatorStatus {
  bool relayState;
  uint8_t ledBrightness;
  uint32_t ledColor;
  uint32_t commandCount;
  uint32_t errorCount;
  uint32_t lastCommandMs;
};

#endif // TYPES_H
