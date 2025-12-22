# 🚀 ESP32 IoT Fleet Management System

<div align="center">

![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Node](https://img.shields.io/badge/node-%3E%3D16.0.0-brightgreen.svg)

**A powerful, real-time web-based IoT platform for managing ESP32 devices with MQTT, gesture control, and intelligent automation.**

[Features](#-features) • [Quick Start](#-quick-start) • [Dashboard](#-dashboard-overview) • [ESP32 Setup](#-esp32-device-setup) • [API Documentation](#-mqtt-api-documentation)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Architecture](#-architecture)
- [Requirements](#-requirements)
- [Quick Start](#-quick-start)
- [Dashboard Overview](#-dashboard-overview)
- [ESP32 Device Setup](#-esp32-device-setup)
- [MQTT API Documentation](#-mqtt-api-documentation)
- [Automation Rules](#-automation-rules)
- [Gesture Control](#-gesture-control)
- [Troubleshooting](#-troubleshooting)
- [Development](#-development)
- [License](#-license)

---

## 🌟 Overview

This is a comprehensive IoT fleet management system built with Node.js that provides real-time monitoring and control of ESP32 devices through MQTT protocol. The system features a modern, responsive web dashboard with advanced capabilities including AI-powered gesture recognition, automated rule execution, and real-time telemetry visualization.

**Perfect for:**
- Smart home automation
- Industrial IoT monitoring
- Educational IoT projects
- Sensor network management
- Remote device control

## ✨ Features

### 🔌 Core Capabilities
- **Built-in MQTT Broker**: Integrated Aedes MQTT broker with TCP (port 1883) and WebSocket (port 3000) support
- **Real-time Dashboard**: Modern, responsive UI with live device monitoring and control
- **Multi-Device Support**: Manage multiple ESP32 sensors and actuators simultaneously
- **Zero Configuration**: Automatic device discovery and registration

### 📊 Monitoring & Control
- **Live Telemetry**: Real-time temperature, humidity, and system health monitoring
- **GPIO Control**: Individual control of up to 8 GPIO pins per actuator device
- **Device Status**: Online/offline detection with connection health indicators
- **Event Logging**: Comprehensive activity logs with export functionality

### 🤖 Intelligent Features
- **AI Gesture Recognition**: Control devices using hand gestures via MediaPipe
- **Smart Automation**: Create conditional rules to automate device actions
- **Rule Engine**: IF-THEN logic with sensor threshold triggers
- **Auto-Toggle**: Automatic state reversal when conditions change

### 🎨 User Interface
- **6 Specialized Tabs**: Dashboard, Device Fleet, Gesture Control, Automation, Events, Documentation
- **Dark Theme**: Modern cyberpunk-inspired design with smooth animations
- **Responsive Layout**: Works seamlessly on desktop, tablet, and mobile
- **Real-time Updates**: Live data refresh without page reload

### 🛠️ Developer Friendly
- **Comprehensive API**: Well-documented MQTT topics and payload formats
- **Code Examples**: Ready-to-use ESP32 Arduino and Python client code
- **Modular Architecture**: Clean separation of concerns for easy customization
- **Debug Support**: Built-in event logging and browser console integration

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Web Browser Client                      │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────────┐    │
│  │  Dashboard  │  │ MQTT Client  │  │ MediaPipe (AI)   │    │
│  └─────────────┘  └──────────────┘  └──────────────────┘    │ 
└────────────────┬────────────────────────────────────────────┘
                 │ HTTP / WebSocket
                 │
┌────────────────▼────────────────────────────────────────────┐
│              Node.js Server (Port 3000)                     │
│  ┌──────────────┐  ┌─────────────────┐  ┌──────────────┐    │
│  │   Express    │  │  Aedes MQTT     │  │  Automation  │    │
│  │   Server     │  │    Broker       │  │    Engine    │    │
│  └──────────────┘  └─────────────────┘  └──────────────┘    │
└────────────────┬────────────────────────────────────────────┘
                 │ MQTT (TCP 1883)
                 │
┌────────────────▼────────────────────────────────────────────┐
│                    ESP32 IoT Devices                        │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │   Sensor    │  │  Actuator   │  │   Sensor + Control  │  │
│  │  (DHT22)    │  │  (Relays)   │  │     (Hybrid)        │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## 📦 Requirements

### Server Requirements
- **Node.js**: Version 16.0.0 or higher
- **npm**: Version 8.x or higher
- **Operating System**: Windows, macOS, or Linux
- **Browser**: Chrome, Edge, Firefox, or Safari (Chrome recommended for gesture recognition)

### ESP32 Requirements
- **ESP32 Board**: ESP32, ESP32-S3, or compatible
- **Arduino Libraries**:
  - `WiFi.h` (built-in)
  - `PubSubClient` (MQTT client)
  - `ArduinoJson` (JSON parsing)
  - `DHT` (for DHT22 sensors - optional)

### Network Requirements
- WiFi network with both server and ESP32 devices connected
- Open port 1883 for MQTT (TCP)
- Open port 3000 for HTTP/WebSocket

## 🚀 Quick Start

### Step 1: Clone the Repository

```bash
git clone https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS.git
cd FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS/Web-Server
```

### Step 2: Install Dependencies

```bash
cd server
npm install
```

### Step 3: Start the Server

```bash
npm start
```

**Expected Output:**
```
============================================================
ESP32 IoT Fleet Management System - Server v2.0
============================================================
[HTTP] Dashboard:       http://localhost:3000
[MQTT] TCP broker:      mqtt://localhost:1883
[MQTT] WebSocket:       ws://localhost:3000/mqtt
============================================================
✓ MQTT broker started on port 1883
✓ WebSocket server started on port 3000
✓ HTTP server started on port 3000
```

### Step 4: Open the Dashboard

Open your web browser and navigate to:
```
http://localhost:3000
```

### Step 5: Find Your Server IP Address

ESP32 devices need your computer's local IP address (not `localhost`):

**Windows:**
```bash
ipconfig
```
Look for `IPv4 Address` under your active network adapter.

**macOS/Linux:**
```bash
ifconfig
# or
ip addr show
```

Example IP: `192.168.1.100`

### Step 6: Configure ESP32 Devices

Use the example code in the **MQTT Docs** tab of the dashboard, or see the [ESP32 Device Setup](#-esp32-device-setup) section below.

## 📁 Project Structure

```
Web-Server/
├── README.md                   # This file
├── server/
│   ├── package.json           # Node.js dependencies
│   ├── server.js              # Main server application
│   │
│   └── public/                # Frontend dashboard
│       ├── index.html         # Main HTML page
│       │
│       ├── css/               # Stylesheets
│       │   ├── variables.css  # Design tokens (colors, spacing, shadows)
│       │   ├── layout.css     # Page layout and grid system
│       │   └── components.css # UI components styling
│       │
│       └── js/                # JavaScript modules
│           ├── events.js      # Event logging system
│           ├── mqtt.js        # MQTT WebSocket client
│           ├── devices.js     # Device management & rendering
│           ├── gestures.js    # AI gesture recognition (MediaPipe)
│           ├── automation.js  # Automation rule engine
│           └── ui.js          # UI controls and navigation
```

## 🎯 Dashboard Overview

The dashboard consists of 6 main tabs:

### 1. 📊 Dashboard Tab
**Quick overview of your IoT fleet**
- **Stats Cards**: Total devices, online devices, sensors, and automation count
- **Device Preview**: Quick view of up to 2 connected devices
- **Real-time Updates**: Live data refresh every second
- **Control Buttons**: Quick access to GPIO controls with toggle switches

**Features:**
- See device status at a glance
- Monitor key metrics (temperature, humidity)
- Quick GPIO control (3 switches per actuator)
- "Control More" button to access all 8 GPIOs

### 2. 🔧 Device Fleet Tab
**Comprehensive device management**
- **Search**: Filter devices by name or ID
- **Device Cards**: Detailed view of each device
  - Device name and type badge
  - Online/offline status indicator
  - Real-time telemetry (temperature, humidity)
  - GPIO toggle switches (for actuators)
  - System info (heap memory, last update time)

**Device Types:**
- **Sensors**: Display environmental data (temperature, humidity)
- **Actuators**: Provide GPIO control switches (8 pins)
- **Hybrid**: Both sensor readings and GPIO control

### 3. 👋 Gesture Control Tab
**AI-powered hand gesture recognition**
- **Camera Feed**: Live video preview with hand tracking overlay
- **5 Supported Gestures**:
  - ✋ **Palm** - All fingers open
  - ✊ **Fist** - All fingers closed
  - 👍 **Thumbs Up** - Approval gesture
  - ☝️ **Point Up** - One finger extended
  - ✌️ **Victory** - Two fingers (peace sign)

**Gesture Rules:**
- Map any gesture to any GPIO action
- Configure target device, GPIO pin, and state (ON/OFF)
- Enable/disable rules with toggle switches
- Edit existing rules
- Visual feedback for detected gestures

**Tips:**
- Use good lighting for best results
- Hold hand 30-60cm from camera
- Keep only one hand visible
- Hold gesture for 1 second to trigger
- 2-second cooldown between triggers

### 4. ⚡ Automation Tab
**Create intelligent automation rules**

**Rule Components:**
- **Name**: Descriptive name for the rule
- **Condition (IF)**: Trigger based on sensor data
  - Source device (sensor)
  - Parameter (temperature, humidity, signal strength)
  - Operator (>, <, >=, <=, ==)
  - Threshold value
- **Action (THEN)**: What to do when condition is met
  - Target device (actuator)
  - GPIO pin (1-8)
  - State (ON/OFF)
- **Auto-toggle**: Automatically turn OFF when condition is no longer met

**Example Rules:**
- "If temperature > 30°C, turn ON fan (GPIO 1)"
- "If humidity < 40%, turn ON humidifier (GPIO 2)"
- "If signal strength < -70 dBm, trigger alert (GPIO 3)"

**Features:**
- Enable/disable rules with toggle switches
- Edit existing rules
- Delete rules
- Active status indicator
- Runs automatically in the background

### 5. 📋 Event Logs Tab
**System activity monitoring**
- **Event Types**:
  - 🔵 Info: General system events
  - ✅ Success: Successful operations
  - ⚠️ Warning: Important notifications
  - ❌ Error: Error messages
  
**Features:**
- Timestamp for each event
- Color-coded event types
- Scrollable timeline
- Export logs as JSON
- Clear all logs button

### 6. 📖 MQTT Docs Tab
**Complete API documentation and code examples**
- **Connection Details**: MQTT broker information
- **Topic Structure**: Subscribe and publish topics
- **Payload Formats**: JSON message structures
- **ESP32 Examples**: Ready-to-use Arduino code
  - MQTT connection setup
  - Sensor data publishing
  - GPIO command handling
  - Complete actuator example
- **Python Client**: paho-mqtt example code

## 🔌 ESP32 Device Setup

### Hardware Configuration

**Supported ESP32 Models:**
- ESP32 DevKit v1
- ESP32-WROOM-32
- ESP32-S2 / ESP32-S3
- ESP32-C3

**For Sensor Devices:**
- DHT22 sensor (temperature & humidity)
- Connect DHT22 data pin to GPIO 4
- VCC → 3.3V, GND → GND

**For Actuator Devices:**
- 8x GPIO outputs (digital pins)
- Can control relays, LEDs, motors, etc.
- Recommended: Use 5V relay modules with optocouplers

### Software Setup

**1. Install Required Libraries** (Arduino IDE)
```
- PubSubClient by Nick O'Leary
- ArduinoJson by Benoit Blanchon
- DHT sensor library by Adafruit (for sensors)
```

**2. Basic ESP32 Code Template**

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Broker (Server IP)
const char* mqtt_server = "192.168.1.100";  // Replace with your server IP
const int mqtt_port = 1883;

// Device configuration
String deviceId = "esp32_001";
String deviceType = "sensor";  // or "actuator"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming messages
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // Parse JSON commands (for actuators)
  StaticJsonDocument<200> doc;
  deserializeJson(doc, message);
  
  if (doc.containsKey("gpio")) {
    int pin = doc["gpio"];
    int state = doc["state"];
    digitalWrite(pin, state);
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect(deviceId.c_str())) {
      Serial.println("connected!");
      // Subscribe to device-specific topic
      String topic = "esp32/" + deviceId + "/commands";
      client.subscribe(topic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  // Your sensor reading or actuator logic here
}
```

**3. Complete Sensor Example** (DHT22)

See the **MQTT Docs tab** in the dashboard for complete working examples including:
- Temperature & humidity monitoring
- Telemetry publishing
- GPIO control for actuators
- JSON payload formatting

**4. Upload and Test**
1. Update WiFi credentials and server IP
2. Upload code to ESP32
3. Open Serial Monitor (115200 baud)
4. Verify connection messages
5. Check dashboard for device appearance

## 📡 MQTT API Reference

### Connection Details

| Parameter | Value |
|-----------|-------|
| **Protocol** | MQTT v3.1.1 |
| **Broker Host** | Server IP address |
| **TCP Port** | 1883 |
| **WebSocket Port** | 3000 (ws://server-ip:3000) |
| **Authentication** | None (configurable) |
| **QoS** | 0 (At most once) |

### Topic Structure

**Device → Server (Publish)**
```
esp32/register                  # Device registration
esp32/{deviceId}/telemetry      # Sensor data
esp32/{deviceId}/status         # Status updates
```

**Server → Device (Subscribe)**
```
esp32/{deviceId}/commands       # GPIO control commands
esp32/broadcast                 # System-wide messages
```

### Message Formats

**1. Device Registration**
```json
Topic: esp32/register

Payload:
{
  "id": "esp32_001",
  "type": "sensor",
  "name": "Living Room Sensor",
  "ip": "192.168.1.150",
  "rssi": -45
}
```

**2. Sensor Telemetry**
```json
Topic: esp32/esp32_001/telemetry

Payload:
{
  "temperature": 25.5,
  "humidity": 60.2,
  "rssi": -45,
  "heap": 285340
}
```

**3. GPIO Control Command** (Server → Actuator)
```json
Topic: esp32/esp32_002/commands

Payload:
{
  "gpio": 1,
  "state": 1
}

// gpio: 1-8 (pin number)
// state: 1 (ON) or 0 (OFF)
```

**4. Status Update**
```json
Topic: esp32/esp32_001/status

Payload:
{
  "online": true,
  "uptime": 3600,
  "rssi": -50
}
```

### Testing MQTT Connection

**Using mosquitto_sub/pub (Linux/Mac):**
```bash
# Subscribe to all ESP32 messages
mosquitto_sub -h 192.168.1.100 -t 'esp32/#' -v

# Publish test message
mosquitto_pub -h 192.168.1.100 -t 'esp32/register' -m '{"id":"test","type":"sensor"}'
```

**Using MQTT.fx or MQTT Explorer (GUI tools):**
1. Download MQTT Explorer: http://mqtt-explorer.com/
2. Connect to server IP on port 1883
3. Subscribe to `esp32/#`
4. Publish test messages

## 🔧 Troubleshooting

### ❌ Devices Not Appearing in Dashboard

**Possible Causes:**
1. **Wrong MQTT server IP**: Double-check the IP address in your ESP32 code
2. **Firewall blocking**: Windows Defender or antivirus may block port 1883
3. **WiFi network**: ESP32 and server must be on the same network
4. **MQTT connection failed**: Check Serial Monitor for error messages

**Solutions:**
```bash
# Check if server is listening on port 1883
netstat -an | findstr "1883"

# Temporarily disable firewall (Windows)
# Control Panel → Windows Defender Firewall → Turn off (for testing only)

# Allow port 1883 through firewall (Windows)
New-NetFirewallRule -DisplayName "MQTT" -Direction Inbound -LocalPort 1883 -Protocol TCP -Action Allow
```

### ⚠️ GPIO Control Not Working

**Checklist:**
- ✅ Device shows "Online" status in dashboard
- ✅ Device type is "actuator" or "hybrid"
- ✅ ESP32 subscribed to correct topic (`esp32/{deviceId}/commands`)
- ✅ GPIO pins configured as OUTPUT in ESP32 code
- ✅ Check browser console (F12) for JavaScript errors

**ESP32 Code Check:**
```cpp
// Make sure you have this in setup()
pinMode(YOUR_GPIO_PIN, OUTPUT);

// Make sure you're subscribed to commands topic
String topic = "esp32/" + deviceId + "/commands";
client.subscribe(topic.c_str());
```

### 🤖 Gesture Recognition Not Working

**Requirements:**
- ✅ Use Chrome or Edge browser (best MediaPipe support)
- ✅ Camera permission granted
- ✅ Good lighting conditions
- ✅ Hand distance: 30-60cm from camera
- ✅ Only one hand visible

**Tips:**
- Clear background for better detection
- Face palm toward camera
- Hold gesture for 1+ seconds
- Check console (F12) for errors
- Try refreshing the page

### 🌐 WebSocket Connection Failed

**Error:** "WebSocket connection to 'ws://...' failed"

**Solution:**
1. Check server is running (`npm start`)
2. Use correct server IP (not localhost if accessing remotely)
3. Clear browser cache
4. Try different browser

### 📊 Sensor Data Not Updating

**Possible Issues:**
1. **DHT22 wiring**: Check connections (VCC, GND, Data pin)
2. **Publishing interval**: Ensure ESP32 publishes regularly
3. **Topic mismatch**: Verify topic name matches `esp32/{deviceId}/telemetry`

**Debug:**
```cpp
// Add this to your ESP32 code to see published messages
Serial.println("Publishing telemetry...");
Serial.println(jsonString);
client.publish(topic.c_str(), jsonString.c_str());
```

## 💻 Development Guide

### Running in Development Mode

```bash
cd server
npm run dev
```

This uses **nodemon** to automatically restart the server when files change.

### Adding New Features

**1. Frontend (Dashboard)**
- CSS: Edit files in `public/css/`
- JavaScript: Add modules in `public/js/`
- HTML: Modify `public/index.html`

**2. Backend (Server)**
- MQTT logic: Edit `server.js`
- Add new routes: Use Express routing

**3. Testing**
```bash
# Test MQTT broker
mosquitto_sub -h localhost -t 'esp32/#' -v

# Test HTTP server
curl http://localhost:3000
```

### Code Style

- **JavaScript**: Use ES6+ features (const, let, arrow functions)
- **CSS**: Follow BEM naming convention
- **Comments**: Document complex logic
- **Indentation**: 2 spaces

### File Organization

```
public/
├── css/
│   ├── variables.css    # 🎨 Design tokens first
│   ├── layout.css       # 📐 Then layout
│   └── components.css   # 🧩 Then components
│
└── js/
    ├── mqtt.js          # 🔌 Core connection
    ├── events.js        # 📝 Logging system
    ├── devices.js       # 🔧 Device management
    ├── automation.js    # ⚡ Rules engine
    ├── gestures.js      # 👋 AI recognition
    └── ui.js            # 🎛️ UI controls (load last)
```

## 📦 Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| **express** | ^4.18.2 | HTTP server and static file serving |
| **aedes** | ^0.46.3 | Lightweight MQTT broker |
| **ws** | ^8.13.0 | WebSocket server implementation |
| **websocket-stream** | ^5.5.2 | Bridge between WebSocket and MQTT |

**Dev Dependencies:**
| Package | Version | Purpose |
|---------|---------|---------|
| **nodemon** | ^2.0.22 | Auto-restart server on file changes |

### Updating Dependencies

```bash
# Check for updates
npm outdated

# Update all packages
npm update

# Update specific package
npm install aedes@latest
```

## 🤝 Contributing

Contributions are welcome! Here's how:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** changes (`git commit -m 'Add AmazingFeature'`)
4. **Push** to branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Ideas for Contributions
- 📱 Mobile-responsive improvements
- 🔐 Add MQTT authentication
- 📊 Advanced charting for sensor data
- 🌐 Multi-language support
- 🔔 Push notifications
- 📧 Email/SMS alerts

## 📄 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2024 ESP32 IoT Fleet Management System

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Acknowledgments

- **MediaPipe** by Google - Hand gesture recognition
- **Aedes** - Lightweight MQTT broker
- **Express.js** - Web framework
- **Arduino & ESP32 Community** - Hardware support

---

<div align="center">

**Made with ❤️ for IoT enthusiasts**

⭐ **Star this repo** if you find it useful!

[Report Bug](https://github.com/yourusername/esp32-iot-fleet/issues) · [Request Feature](https://github.com/yourusername/esp32-iot-fleet/issues)

</div>
