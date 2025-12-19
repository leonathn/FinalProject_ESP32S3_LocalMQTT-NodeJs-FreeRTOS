/**
 * @file mqtt_handler.h
 * @brief MQTT client operations (subscribe/publish)
 * 
 * Manages MQTT broker connection and message handling:
 * - Subscribes to GPIO control commands
 * - Publishes device status/heartbeat
 * - Handles reconnection logic
 */

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

/**
 * @brief Connect to MQTT broker
 * 
 * Establishes connection to MQTT broker using configured server and port.
 * Subscribes to command topic: device/<device_id>/gpio/set
 * Publishes online status on successful connection.
 */
void connectMQTT();

/**
 * @brief MQTT message callback handler
 * 
 * Called when message received on subscribed topic.
 * Parses JSON command and updates GPIO states.
 * 
 * @param topic MQTT topic path
 * @param payload Message payload (JSON)
 * @param length Payload byte length
 */
void mqttCallback(char* topic, byte* payload, unsigned int length);

/**
 * @brief Publish device status to MQTT
 * 
 * Sends JSON status message containing:
 * - Online state
 * - Current GPIO states
 * - System metrics (heap, uptime)
 * 
 * Published to: devices/<device_id>/status
 */
void publishStatus();

#endif // MQTT_HANDLER_H
