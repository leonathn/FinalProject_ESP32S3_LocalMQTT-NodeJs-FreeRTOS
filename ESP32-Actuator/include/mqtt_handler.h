/*
 * MQTT Connection and Message Handling
 */

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include "types.h"

void connectMQTT();
void mqttCallback(char* topic, uint8_t* payload, unsigned int length);
void publishStatus();
void publishTelemetry();

#endif // MQTT_HANDLER_H
