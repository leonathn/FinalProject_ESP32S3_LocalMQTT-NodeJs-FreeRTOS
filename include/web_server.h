/**
 * @file web_server.h
 * @brief Web server and configuration portal
 * 
 * Provides HTTP web server for device configuration and monitoring.
 * Serves responsive HTML interface and REST API endpoints.
 * 
 * Main Features:
 * - WiFi network scanner and configuration
 * - MQTT broker discovery and setup
 * - Live sensor data display
 * - System diagnostics
 * - Configuration management
 * 
 * Accessible at:
 * - AP Mode: http://192.168.4.1
 * - STA Mode: http://<device_ip> or http://<device_id>.local
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

/**
 * @brief Initialize and start web server
 * 
 * Registers all HTTP route handlers and starts server on port 80.
 * Called once during setup().
 */
void setupWebServer();

/**
 * @brief Handle root page request (GET /)
 * 
 * Serves main configuration HTML interface with embedded JavaScript.
 * Responsive design with tabs for WiFi, MQTT, and sensor configuration.
 */
void handleRoot();

/**
 * @brief Handle configuration save (POST /save)
 * 
 * Saves WiFi and MQTT settings to NVS and triggers device reboot.
 * Accepts form data: ssid, password, mqtt_server, mqtt_port, token.
 */
void handleSave();

/**
 * @brief Handle status API request (GET /api/status)
 * 
 * Returns JSON with current device status (mode, connections, uptime, heap).
 */
void handleStatus();

/**
 * @brief Handle diagnostics API request (GET /api/diagnostics/run)
 * 
 * Runs system diagnostics and returns results as JSON.
 */
void handleDiagnostics();

/**
 * @brief Handle WiFi scan request (GET /api/wifi/scan)
 * 
 * Scans for available WiFi networks and returns list with RSSI and encryption.
 */
void handleWiFiScan();

/**
 * @brief Handle sensor data request (GET /api/sensors)
 * 
 * Returns current DHT20 sensor readings and sensor health statistics.
 */
void handleSensors();

/**
 * @brief Handle configuration reset (POST /api/reset)
 * 
 * Clears all stored configuration and reboots to AP mode.
 */
void handleReset();

/**
 * @brief Handle MQTT broker scan (GET /api/mqtt/scan)
 * 
 * Uses mDNS to discover MQTT brokers on local network.
 * Returns list of discovered brokers with IP, port, and hostname.
 */
void handleMQTTScan();

#endif // WEB_SERVER_H
