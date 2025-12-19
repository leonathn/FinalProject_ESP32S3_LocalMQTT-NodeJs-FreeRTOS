/**
 * @file diagnostics.h
 * @brief System diagnostics and health checks
 * 
 * Performs comprehensive system diagnostics including:
 * - I2C bus scan for connected devices
 * - DHT20 sensor validation and test reading
 * - NeoPixel LED functional test
 * 
 * Results stored in global 'lastDiagnostics' and published via MQTT.
 * Can be triggered via web API or MQTT command.
 */

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

/**
 * @brief Run full system diagnostics
 * 
 * Scans I2C bus, tests DHT20 sensor, validates NeoPixel.
 * Updates global 'lastDiagnostics' structure with results.
 * Publishes results to MQTT topic: devices/<device_id>/diagnostics
 * 
 * Thread-safe: Acquires i2cMutex for sensor access.
 */
void runDiagnostics();

#endif // DIAGNOSTICS_H
