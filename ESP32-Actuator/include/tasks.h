/**
 * @file tasks.h
 * @brief FreeRTOS task declarations
 * 
 * Declares FreeRTOS tasks for concurrent operation:
 * - MQTT Task: Handles MQTT communication and GPIO commands
 * - Status Task: Publishes periodic heartbeat/status messages
 * - UI Task: Updates NeoPixel LED status indicator
 */

#ifndef TASKS_H
#define TASKS_H

/**
 * @brief MQTT communication task
 * 
 * Priority: 3 (Highest)
 * Handles MQTT client loop, subscriptions, and GPIO command processing.
 * 
 * @param pvParameters Task parameters (unused)
 */
void TaskMQTT(void *pvParameters);

/**
 * @brief Status publishing task
 * 
 * Priority: 2 (Medium)
 * Publishes device status/heartbeat every 20 seconds.
 * 
 * @param pvParameters Task parameters (unused)
 */
void TaskStatus(void *pvParameters);

/**
 * @brief UI update task
 * 
 * Priority: 1 (Low)
 * Updates NeoPixel LED every 500ms to reflect connection state.
 * 
 * @param pvParameters Task parameters (unused)
 */
void TaskUI(void *pvParameters);

#endif // TASKS_H
