/*
 * Web Server and Captive Portal
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

void setupWebServer();
void handleRoot();
void handleSave();
void handleStatus();
void handleWiFiScan();
void handleGPIO();
void handleReset();
void handleMQTTScan();

#endif // WEB_SERVER_H
