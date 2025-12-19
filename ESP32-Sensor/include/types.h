/**
 * @file types.h
 * @brief Custom data types, structures, and enumerations
 * 
 * This file defines all custom data types used throughout the application:
 * - Sensor data structures for readings and events
 * - Diagnostic data structures
 * - State tracking enumerations
 * - Telemetry data formats
 * 
 * These types ensure type-safe data passing between FreeRTOS tasks and
 * provide clear data contracts for inter-task communication.
 */

#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// ========== DIAGNOSTICS ==========
/**
 * @struct DiagnosticsData
 * @brief System health check results
 * 
 * Contains the results of running system diagnostics including I2C bus scan,
 * sensor validation, and peripheral tests. Used by the diagnostics module
 * to report system health via MQTT and web API.
 */
struct DiagnosticsData {
  bool i2cOk;              // I2C bus operational status
  String i2cDevices;       // Comma-separated list of detected I2C addresses (e.g., "0x38")
  bool dht20Ok;            // DHT20 sensor responsive and providing valid data
  float temperature;       // Last successful temperature reading from DHT20 (°C)
  float humidity;          // Last successful humidity reading from DHT20 (%)
  bool neopixelOk;         // NeoPixel LED functioning (basic test)
  String errorMsg;         // Human-readable error message if diagnostics fail
};

// ========== TELEMETRY ==========
/**
 * @struct TelemetryData
 * @brief Sensor readings and system telemetry data
 * 
 * Primary data structure passed from sensor task to MQTT task via queue.
 * Contains sensor readings plus system health metrics.
 * Published to MQTT topic: devices/<device_id>/telemetry
 */
struct TelemetryData {
  float temperature;       // Temperature in Celsius from DHT20
  float humidity;          // Relative humidity in % from DHT20
  uint32_t heap;           // Free heap memory in bytes
  uint32_t uptime;         // System uptime in seconds
  uint8_t quality;         // Data quality score: 0 (bad) to 100 (perfect)
  bool valid;              // True if sensor data is valid and trustworthy
};

// ========== SENSOR EVENT ==========
/**
 * @struct SensorEvent
 * @brief Sensor event notifications for logging
 * 
 * Used to communicate sensor events (readings, errors, warnings) between tasks.
 * Queued events can be logged or published for debugging.
 */
struct SensorEvent {
  enum Type { READING, ERROR, WARNING, INFO } type;  // Event severity/type
  float temperature;       // Associated temperature reading
  float humidity;          // Associated humidity reading
  uint32_t timestamp;      // Event timestamp (milliseconds)
  char message[64];        // Human-readable event message
};

// ========== SENSOR BANDS ==========
/**
 * @enum TempBand
 * @brief Temperature range classification
 * 
 * Categorizes temperature readings into bands for triggering alerts
 * or changing system behavior based on environmental conditions.
 */
enum class TempBand {
  COLD,      // < 18°C - Below comfortable temperature
  NORMAL,    // 18-25°C - Normal room temperature range
  HOT,       // 25-30°C - Warm, potentially uncomfortable
  CRITICAL   // > 30°C - High temperature alert
};

/**
 * @enum HumBand
 * @brief Humidity range classification
 * 
 * Categorizes humidity levels for environmental monitoring and alerts.
 */
enum class HumBand {
  DRY,       // < 30% - Low humidity, dry air
  COMFORT,   // 30-60% - Comfortable humidity range
  HUMID,     // 60-80% - High humidity
  WET        // > 80% - Very high humidity, potential condensation
};

// ========== SENSOR STATE ==========
/**
 * @struct SensorState
 * @brief Persistent sensor state tracking
 * 
 * Tracks sensor health, reading statistics, and state changes over time.
 * Used for monitoring sensor reliability and detecting degradation.
 */
struct SensorState {
  TempBand lastTempBand;   // Last recorded temperature band
  HumBand lastHumBand;     // Last recorded humidity band
  uint32_t readCount;      // Total successful sensor reads since boot
  uint32_t errorCount;     // Total failed sensor reads since boot
  uint32_t lastReadMs;     // Timestamp of last successful read (milliseconds)
  bool initialized;        // True if sensor has been successfully initialized
};

#endif // TYPES_H
