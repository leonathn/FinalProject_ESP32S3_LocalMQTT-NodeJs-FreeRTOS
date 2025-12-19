/**
 * @file web_server.h
 * @brief HTTP web server for device configuration portal
 * 
 * Provides web-based configuration interface accessible in AP mode.
 * Serves HTML forms and REST API endpoints for:
 * - WiFi network configuration
 * - MQTT broker settings
 * - Device status viewing
 * - Configuration save/reboot
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

/**
 * @brief Start HTTP web server
 * 
 * Initializes web server routes and handlers.
 * Serves configuration portal and API endpoints.
 * Call during setup() after WiFi initialization.
 */
void startWebServer();

/**
 * @brief Process incoming HTTP requests
 * 
 * Must be called regularly in loop() or task to handle client requests.
 * Non-blocking, returns immediately if no clients.
 */
void handleWebServer();

#endif // WEB_SERVER_H
