/**
 * @file types.h
 * @brief Custom data types, structures, and enumerations
 * 
 * This file defines all custom data types used throughout the actuator application:
 * - GPIO state structures
 * - Status data formats
 * - Device state tracking
 * 
 * These types ensure type-safe data passing between FreeRTOS tasks and
 * provide clear data contracts for inter-task communication.
 */

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// ========== GPIO STATE ==========
/**
 * @struct GPIOState
 * @brief Current state of all 8 GPIO channels
 * 
 * Tracks the on/off state of each GPIO channel.
 * Published to MQTT in status messages.
 */
struct GPIOState {
  bool ch1;  // Channel 1 state (true=ON, false=OFF)
  bool ch2;  // Channel 2 state
  bool ch3;  // Channel 3 state
  bool ch4;  // Channel 4 state
  bool ch5;  // Channel 5 state
  bool ch6;  // Channel 6 state
  bool ch7;  // Channel 7 state
  bool ch8;  // Channel 8 state
};

// ========== STATUS DATA ==========
/**
 * @struct StatusData
 * @brief Device status and health information
 * 
 * Published periodically to MQTT topic: devices/<device_id>/status
 * Contains connection state, GPIO states, and system metrics.
 */
struct StatusData {
  bool online;           // Device online status (always true when publishing)
  GPIOState gpioStates;  // Current state of all GPIO channels
  uint32_t heap;         // Free heap memory in bytes
  uint32_t uptime;       // System uptime in seconds
  uint32_t timestamp;    // Message timestamp
};

#endif // TYPES_H
