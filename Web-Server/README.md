# Web Server - ESP32 IoT Fleet Management

A Node.js web server with built-in MQTT broker for managing ESP32 IoT devices. Provides a real-time dashboard for device monitoring, GPIO control, automation rules, and AI gesture recognition.

## Features

- **MQTT Broker**: Built-in Aedes broker (TCP port 1883, WebSocket port 3000)
- **Real-time Dashboard**: Monitor sensor data, control actuators, view event logs
- **Gesture Control**: MediaPipe hand tracking to control devices with hand gestures
- **Automation**: Create rules to trigger GPIO actions based on sensor thresholds

## Requirements

- Node.js 16.0.0 or higher
- npm 8.x or higher

## Quick Start

### 1. Install Dependencies

```bash
cd Web-Server/server
npm install
```

### 2. Start the Server

```bash
npm start
```

You should see:
```
============================================================
ESP32 IoT Fleet Management System - Server v2.0
============================================================
[HTTP] Dashboard:       http://localhost:3000
[MQTT] TCP broker:      mqtt://localhost:1883
[MQTT] WebSocket:       ws://localhost:3000/mqtt
============================================================
```

### 3. Open Dashboard

Open your browser and go to: **http://localhost:3000**

### 4. Connect ESP32 Devices

Configure your ESP32 devices to connect to the MQTT broker:
- **MQTT Server**: Your computer's IP address (e.g., `192.168.1.100`)
- **MQTT Port**: `1883`

> **Note**: Use your computer's local IP address, not `localhost` or `127.0.0.1`. ESP32 devices cannot resolve these addresses.

To find your IP address:
- **Windows**: Run `ipconfig` in Command Prompt
- **Mac/Linux**: Run `ifconfig` or `ip addr`

## Project Structure

```
Web-Server/
├── server/
│   ├── package.json        # Dependencies
│   ├── server.js           # Main server file
│   │
│   ├── public/             # Dashboard frontend
│   │   ├── index.html      # Main page
│   │   ├── css/
│   │   │   ├── variables.css   # Colors and design tokens
│   │   │   ├── layout.css      # Page layout
│   │   │   └── components.css  # UI components
│   │   └── js/
│   │       ├── events.js       # Event logging
│   │       ├── mqtt.js         # MQTT client
│   │       ├── devices.js      # Device management
│   │       ├── gestures.js     # Gesture recognition
│   │       ├── automation.js   # Automation rules
│   │       └── ui.js           # UI controls
│   │
│   └── ai_webcam/          # MediaPipe gesture model
│       └── model/
```

## Dashboard Tabs

### Devices Tab
View all connected ESP32 devices with:
- Online/offline status
- Device type (sensor or actuator)
- IP address and WiFi signal strength
- Live temperature and humidity readings

### Control Tab
Control actuator GPIO pins:
1. Select a device from the dropdown
2. Click ON/OFF buttons for each GPIO channel (1-8)

### Automation Tab
Create rules that trigger automatically:
1. Click **Add Rule**
2. Set a condition (e.g., temperature > 30)
3. Choose target device and GPIO
4. Set action (ON/OFF)
5. Optional: Enable auto-toggle to turn off after a delay

### Gestures Tab
Control devices with hand gestures:
1. Click **Start Camera** and allow camera access
2. Add gesture rules to map gestures to GPIO actions
3. Supported gestures:
   - **Fist**: All fingers closed
   - **Palm**: All fingers open
   - **Peace**: Index and middle fingers extended
   - **Thumbs Up**: Thumb pointing up
   - **Thumbs Down**: Thumb pointing down

### Events Tab
View system activity log:
- Device connections/disconnections
- Sensor readings
- GPIO commands
- Automation triggers
- Export logs as JSON

## MQTT Topics

The server uses these MQTT topic patterns:

| Topic | Direction | Purpose |
|-------|-----------|---------|
| `devices/{id}/telemetry` | ESP32 → Server | Sensor data (temperature, humidity) |
| `devices/{id}/status` | ESP32 → Server | Device online/offline status |
| `device/{id}/gpio/set` | Server → ESP32 | GPIO control commands |

### Telemetry Message Format

```json
{
  "tC": 25.5,
  "tF": 77.9,
  "rh": 60.2,
  "timestamp": 1703012345678
}
```

### GPIO Command Format

```json
{
  "gpio": 1,
  "state": 1
}
```

## Troubleshooting

### Devices not appearing in dashboard
1. Check that ESP32 is configured with correct MQTT server IP
2. Verify your computer's firewall allows port 1883
3. Check the terminal for connection messages

### GPIO control not working
1. Ensure the actuator device shows "Online" status
2. Check browser console (F12) for errors
3. Verify the device is subscribed to the correct topic

### Gesture recognition not working
1. Use Chrome or Edge browser (best MediaPipe support)
2. Ensure good lighting
3. Hold hand 30-60cm from camera
4. Keep only one hand visible

## Development

For development with auto-reload:

```bash
npm run dev
```

This uses nodemon to restart the server when files change.

## Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| express | 4.18.2 | HTTP server |
| aedes | 0.51.3 | MQTT broker |
| ws | 8.16.0 | WebSocket server |
| websocket-stream | 5.5.2 | WebSocket-MQTT bridge |
| cors | 2.8.5 | Cross-origin support |

## License

MIT License
