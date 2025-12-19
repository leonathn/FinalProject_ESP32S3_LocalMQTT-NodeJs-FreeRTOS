/**
 * @file mqtt_handler.h
 * @brief MQTT client connection and message handling
 * 
 * Manages MQTT broker connection, topic subscriptions, message publishing,
 * and incoming command processing.
 * 
 * Published Topics:
 * - devices/<device_id>/status - Device online status (retained)
 * - devices/<device_id>/telemetry - Sensor readings (every 1s)
 * - devices/<device_id>/pair - Pairing token
 * - devices/<device_id>/diagnostics - System health
 * 
 * Subscribed Topics:
 * - devices/<device_id>/cmd - Remote commands (reboot, diagnostics)
 * - devices/<device_id>/config - Configuration updates (reserved)
 */

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "types.h"

/**
 * @brief Connect to MQTT broker
 * 
 * Establishes connection using configured server/port.
 * Subscribes to command and config topics.
 * Publishes initial status and pairing messages.
 */
void connectMQTT();

/**
 * @brief MQTT message callback handler
 * @param topic Topic name of received message
 * @param payload Message payload bytes
 * @param length Payload length in bytes
 * 
 * Processes incoming MQTT messages (commands, config updates).
 * Parses JSON payloads and executes corresponding actions.
 */
void mqttCallback(char* topic, byte* payload, unsigned int length);

/**
 * @brief Publish device online status
 * 
 * Publishes retained status message with IP, RSSI, and mode.
 * Topic: devices/<device_id>/status
 */
void publishStatus();

/**
 * @brief Publish telemetry data
 * @param data Reference to telemetry data structure
 * @return True if publish succeeded, false otherwise
 * 
 * Publishes sensor readings and system metrics.
 * Topic: devices/<device_id>/telemetry
 */
bool publishTelemetry(TelemetryData &data);

/**
 * @brief Publish device pairing token
 * 
 * Sends pairing token for server-side device registration.
 * Topic: devices/<device_id>/pair
 */
void publishPairing();

#endif // MQTT_HANDLER_H
