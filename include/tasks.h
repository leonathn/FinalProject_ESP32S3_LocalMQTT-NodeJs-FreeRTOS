/**
 * @file tasks.h
 * @brief FreeRTOS task function declarations
 * 
 * Defines three concurrent tasks running on ESP32 dual cores:
 * 
 * TaskSensors (Core 0, Priority 1):
 * - Reads DHT20 sensor every 1 second
 * - Validates data quality
 * - Queues telemetry for MQTT transmission
 * 
 * TaskUI (Core 0, Priority 1):
 * - Updates NeoPixel LED every 500ms
 * - Visual indication of connection state
 * 
 * TaskMQTT (Core 1, Priority 2):
 * - Processes telemetry queue
 * - Publishes data to MQTT broker
 * - Handles MQTT reconnection
 */

#ifndef TASKS_H
#define TASKS_H

/**
 * @brief Sensor reading task
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Continuously reads DHT20 temperature/humidity sensor with mutex protection.
 * Validates readings and queues valid data for MQTT transmission.
 */
void TaskSensors(void *pvParameters);

/**
 * @brief User interface update task
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Updates NeoPixel LED color based on system connection state.
 * Provides visual feedback to user.
 */
void TaskUI(void *pvParameters);

/**
 * @brief MQTT communication task
 * @param pvParameters Unused FreeRTOS parameter
 * 
 * Manages MQTT connection, processes telemetry queue, publishes data.
 * Handles automatic reconnection on connection loss.
 */
void TaskMQTT(void *pvParameters);

#endif // TASKS_H
