/*
 * FreeRTOS Tasks - Actuator Version
 */

#ifndef TASKS_H
#define TASKS_H

void TaskActuator(void *pvParameters);
void TaskUI(void *pvParameters);
void TaskMQTT(void *pvParameters);

#endif // TASKS_H
