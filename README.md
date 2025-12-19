# ESP32 IoT Fleet Management System

**A complete IoT ecosystem featuring ESP32-S3 sensor/actuator nodes with local MQTT broker, Node.js web server, and AI gesture control**

![Project Version](https://img.shields.io/badge/version-2.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-green)
![Framework](https://img.shields.io/badge/framework-FreeRTOS-orange)
![License](https://img.shields.io/badge/license-MIT-blue)

---

## 📋 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Key Features](#-key-features)
- [Project Structure](#-project-structure)
- [Hardware Requirements](#-hardware-requirements)
- [Software Setup](#-software-setup)
- [Quick Start Guide](#-quick-start-guide)
- [Configuration](#-configuration)
- [MQTT Communication](#-mqtt-communication)
- [Web Dashboard](#-web-dashboard)
- [Gesture Control](#-gesture-control)
- [Automation](#-automation)
- [Development](#-development)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🎯 Overview

This is a **production-ready, enterprise-grade IoT system** built with ESP32-S3 microcontrollers, featuring:

- **Dual Device Types**: Separate sensor and actuator firmware for specialized functionality
- **Local MQTT Broker**: Private, secure communication without cloud dependency
- **Real-time Dashboard**: Web-based monitoring and control interface
- **AI Gesture Control**: MediaPipe-powered hand gesture recognition for touchless control
- **FreeRTOS Multithreading**: Efficient task management with semaphores and queues
- **WiFi Manager**: Captive portal for easy device configuration
- **Automation Engine**: Rule-based automation for smart home scenarios

### Use Cases

- **Home Automation**: Temperature monitoring and climate control
- **Industrial IoT**: Sensor monitoring with remote actuator control
- **Smart Agriculture**: Environmental monitoring and irrigation control
- **Education**: Learn IoT, MQTT, FreeRTOS, and embedded systems
- **Prototyping**: Rapid development of IoT projects

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        Web Dashboard                            │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐          │
│  │Dashboard │ │  Devices │ │ Gestures │ │Automation│          │
│  └──────────┘ └──────────┘ └──────────┘ └──────────┘          │
│       │            │             │            │                 │
│       └────────────┴─────────────┴────────────┘                 │
│                         │                                        │
│              ┌──────────▼──────────┐                            │
│              │   Node.js Server     │                            │
│              │  - Express HTTP     │                            │
│              │  - Aedes MQTT       │                            │
│              │  - WebSocket        │                            │
│              └──────────┬──────────┘                            │
│                         │                                        │
└─────────────────────────┼────────────────────────────────────────┘
                          │ MQTT
              ┌───────────┴───────────┐
              │                       │
     ┌────────▼────────┐    ┌────────▼────────┐
     │  ESP32-Sensor   │    │ ESP32-Actuator  │
     │                 │    │                 │
     │ ┌─────────────┐ │    │ ┌─────────────┐ │
     │ │ DHT20       │ │    │ │ 8-Ch GPIO   │ │
     │ │ Temp/Humid  │ │    │ │ Relay Ctrl  │ │
     │ └─────────────┘ │    │ └─────────────┘ │
     │ ┌─────────────┐ │    │ ┌─────────────┐ │
     │ │ NeoPixel    │ │    │ │ NeoPixel    │ │
     │ │ Status LED  │ │    │ │ Status LED  │ │
     │ └─────────────┘ │    │ └─────────────┘ │
     │ ┌─────────────┐ │    │ ┌─────────────┐ │
     │ │ WiFi Config │ │    │ │ WiFi Config │ │
     │ │ Web Portal  │ │    │ │ Web Portal  │ │
     │ └─────────────┘ │    │ └─────────────┘ │
     └─────────────────┘    └─────────────────┘
          FreeRTOS               FreeRTOS
       3 Tasks + Mutex       3 Tasks + Queue
```

---

## ✨ Key Features

### ESP32 Sensor Node
- ✅ **DHT20 I²C Sensor** - Temperature and humidity monitoring
- ✅ **FreeRTOS Tasks** - Multi-threaded sensor reading, MQTT publishing, UI updates
- ✅ **MQTT Telemetry** - Real-time data publishing every 5 seconds
- ✅ **WiFi Manager** - Captive portal configuration (AP mode)
- ✅ **NeoPixel Status LED** - Visual feedback for WiFi/MQTT status
- ✅ **System Diagnostics** - Built-in health checks and I²C scanning
- ✅ **NVS Storage** - Non-volatile configuration persistence
- ✅ **Factory Reset** - Hold boot button for 3 seconds

### ESP32 Actuator Node
- ✅ **8-Channel GPIO Control** - Relay, LED, motor, solenoid control
- ✅ **MQTT Command Subscription** - Receives commands via MQTT
- ✅ **Command Queue** - FreeRTOS queue for buffering incoming commands
- ✅ **Independent Firmware** - Separate from sensor for specialization
- ✅ **WiFi Manager** - Same captive portal as sensor
- ✅ **NeoPixel Status LED** - Visual feedback
- ✅ **Factory Reset** - Hold boot button for 3 seconds

### Node.js Web Server
- ✅ **Built-in MQTT Broker** - Aedes broker (TCP + WebSocket)
- ✅ **Real-time Dashboard** - Live device monitoring
- ✅ **Device Fleet Management** - Track all connected devices
- ✅ **GPIO Control Panel** - Remote actuator control
- ✅ **Event Logging** - System-wide event tracking
- ✅ **Automation Rules** - Threshold-based automation
- ✅ **AI Gesture Control** - MediaPipe hand gesture recognition
- ✅ **Responsive Design** - Modern, mobile-friendly UI

### AI Gesture Recognition
- ✅ **5 Hand Gestures** - Open palm, fist, peace, thumbs up, pointing
- ✅ **MediaPipe Hands** - Google's ML model for hand tracking
- ✅ **WebRTC Camera** - Browser-based camera access
- ✅ **Gesture Mapping** - Assign gestures to GPIO actions
- ✅ **Confidence Threshold** - 85% minimum confidence
- ✅ **Cooldown Timer** - 2-second debounce

---

## 📂 Project Structure

```
Final_Project_Embedded_Systems/
│
├── ESP32-Sensor/                # Sensor firmware (DHT20)
│   ├── include/
│   │   ├── config.h            # Pin definitions and constants
│   │   ├── types.h             # Data structures and enums
│   │   ├── globals.h           # Global variables and objects
│   │   ├── config_manager.h    # NVS storage functions
│   │   ├── wifi_manager.h      # WiFi AP/STA management
│   │   ├── mqtt_handler.h      # MQTT client functions
│   │   ├── neopixel_handler.h  # LED status indicator
│   │   ├── diagnostics.h       # System diagnostics
│   │   ├── web_server.h        # Web server and API
│   │   └── tasks.h             # FreeRTOS task definitions
│   ├── src/
│   │   ├── main.cpp            # Main program entry point
│   │   ├── config_manager.cpp  # Configuration persistence
│   │   ├── wifi_manager.cpp    # WiFi connection handling
│   │   ├── mqtt_handler.cpp    # MQTT messaging
│   │   ├── neopixel_handler.cpp # LED control
│   │   ├── diagnostics.cpp     # System health checks
│   │   ├── web_server.cpp      # Web UI and API handlers
│   │   └── tasks.cpp           # FreeRTOS task implementations
│   ├── boards/
│   │   └── yolo_uno.json       # Custom board configuration
│   ├── platformio.ini          # PlatformIO configuration
│   └── README.md               # Sensor-specific documentation
│
├── ESP32-Actuator/              # Actuator firmware (GPIO control)
│   ├── include/
│   │   ├── config.h            # Pin definitions and constants
│   │   ├── types.h             # Data structures and enums
│   │   ├── globals.h           # Global variables and objects
│   │   ├── config_manager.h    # NVS storage functions
│   │   ├── wifi_manager.h      # WiFi AP/STA management
│   │   ├── mqtt_handler.h      # MQTT client functions
│   │   ├── neopixel_handler.h  # LED status indicator
│   │   ├── web_server.h        # Web server and API
│   │   └── tasks.h             # FreeRTOS task definitions
│   ├── src/
│   │   ├── main.cpp            # Main program entry point
│   │   ├── config_manager.cpp  # Configuration persistence
│   │   ├── wifi_manager.cpp    # WiFi connection handling
│   │   ├── mqtt_handler.cpp    # MQTT messaging
│   │   ├── neopixel_handler.cpp # LED control
│   │   ├── web_server.cpp      # Web UI and API handlers
│   │   └── tasks.cpp           # FreeRTOS task implementations
│   ├── platformio.ini          # PlatformIO configuration
│   ├── UPLOAD_GUIDE.md         # Upload instructions
│   └── README.md               # Actuator-specific documentation
│
├── Web-Server/                  # Node.js server and dashboard
│   ├── server/
│   │   ├── server.js           # Express + Aedes MQTT broker
│   │   ├── package.json        # Node.js dependencies
│   │   └── public/             # Web dashboard frontend
│   │       ├── index.html      # Main page
│   │       ├── css/
│   │       │   ├── variables.css   # Design tokens
│   │       │   ├── layout.css      # Page layout
│   │       │   └── components.css  # UI components
│   │       └── js/
│   │           ├── mqtt.js         # MQTT client
│   │           ├── devices.js      # Device management
│   │           ├── gestures.js     # Gesture recognition
│   │           ├── automation.js   # Automation rules
│   │           ├── events.js       # Event logging
│   │           └── ui.js           # UI controls
│   └── README.md               # Server documentation
│
└── README.md                    # This file
```

---

## 🔧 Hardware Requirements

### ESP32 Sensor Node
| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP32-S3 DevKit | 1 | Main microcontroller (dual-core, WiFi) |
| DHT20 I²C Sensor | 1 | Temperature + Humidity sensor |
| WS2812B NeoPixel | 1 | RGB Status LED |
| Breadboard | 1 | For prototyping |
| Jumper Wires | 8 | Male-to-male or male-to-female |

**Pin Assignments:**
- SDA: GPIO 11 (I²C Data)
- SCL: GPIO 12 (I²C Clock)
- NeoPixel: GPIO 45
- Reset Button: GPIO 0 (Boot button)

### ESP32 Actuator Node
| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP32-S3 DevKit | 1 | Main microcontroller (dual-core, WiFi) |
| WS2812B NeoPixel | 1 | RGB Status LED |
| Relay Module (optional) | 1-8 | For switching high-power loads |
| Breadboard | 1 | For prototyping |
| Jumper Wires | 10 | Male-to-male or male-to-female |

**GPIO Output Pins (8 channels):**
- GPIO 5, 6, 7, 8 (Channels 1-4)
- GPIO 9, 10, 21, 38 (Channels 5-8)
- NeoPixel: GPIO 45
- Reset Button: GPIO 0 (Boot button)

### Server Requirements
- Computer running Windows/Mac/Linux
- Node.js 16.0.0 or higher
- npm 8.x or higher
- Webcam (for gesture control)

---

## 💻 Software Setup

### 1. Install Development Tools

#### For ESP32 Development
1. **Install Visual Studio Code**
   - Download from: https://code.visualstudio.com/

2. **Install PlatformIO IDE Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install
   - Restart VS Code

#### For Node.js Server
1. **Install Node.js**
   - Download from: https://nodejs.org/
   - Choose LTS version (16.x or higher)
   - Verify installation: `node --version` and `npm --version`

### 2. Clone Repository

```bash
git clone https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS.git
cd FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS
```

### 3. Setup ESP32 Sensor

```bash
cd ESP32-Sensor
```

1. **Connect ESP32-S3** to your computer via USB
2. **Identify COM Port**:
   - Windows: Check Device Manager → Ports (COM & LPT)
   - Mac/Linux: `ls /dev/tty*`
3. **Update platformio.ini** with your COM port:
   ```ini
   upload_port = COM3  ; Change to your port
   monitor_port = COM3
   ```
4. **Build and Upload**:
   - Click PlatformIO icon (alien head) in VS Code
   - Click "Upload and Monitor" under ESP32-Sensor

### 4. Setup ESP32 Actuator

```bash
cd ../ESP32-Actuator
```

1. **Connect second ESP32-S3** (or reuse same device)
2. **Update platformio.ini** with COM port
3. **Build and Upload**:
   - Click "Upload and Monitor" under ESP32-Actuator

### 5. Setup Web Server

```bash
cd ../Web-Server/server
npm install
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

---

## 🚀 Quick Start Guide

### Step 1: Configure ESP32 Devices

Both sensor and actuator use the same WiFi configuration process:

1. **Power on ESP32** - NeoPixel will show **orange** (booting)
2. **Wait for AP mode** - NeoPixel will show **blue** (no config found)
3. **Connect to WiFi hotspot**:
   - SSID: `ESP32-IOT-SENSOR-XXXX` or `ESP32-IOT-ACTUATOR-XXXX`
   - Password: `12345678`
4. **Captive portal opens automatically**
   - If not, browse to: `http://192.168.4.1`
5. **Configure device**:
   - WiFi SSID: Your home WiFi network
   - WiFi Password: Your WiFi password
   - MQTT Server: Your computer's IP address (e.g., `192.168.1.100`)
   - MQTT Port: `1883`
   - Pairing Token: Any string (e.g., `my-token-123`)
6. **Submit configuration** - Device will reboot
7. **NeoPixel status**:
   - **Green**: Connected to WiFi and MQTT
   - **Yellow**: Connected to WiFi, not MQTT
   - **Red**: Connection error

### Step 2: Find Your Computer's IP Address

**Windows:**
```bash
ipconfig
```
Look for "IPv4 Address" under your WiFi adapter

**Mac/Linux:**
```bash
ifconfig
# or
ip addr show
```

⚠️ **Important**: Do NOT use `localhost` or `127.0.0.1` - ESP32 devices need your actual local IP!

### Step 3: Access Dashboard

Open browser and go to: **http://localhost:3000**

You should see:
- Device count showing your connected ESP32s
- Real-time sensor data (if sensor is connected)
- Online status for each device

---

## ⚙️ Configuration

### ESP32 Configuration (Both Sensor and Actuator)

Configuration is stored in NVS (Non-Volatile Storage) and persists across reboots.

**Factory Reset:**
1. Hold Boot button (GPIO 0) for 3 seconds
2. NeoPixel will blink red
3. Device clears config and reboots to AP mode

**Web Configuration Interface:**
- Accessible in AP mode at `http://192.168.4.1`
- Fields:
  - WiFi SSID
  - WiFi Password
  - MQTT Server IP
  - MQTT Port (default: 1883)
  - Pairing Token

### Web Server Configuration

Edit `Web-Server/server/server.js`:

```javascript
const HTTP_PORT = 3000;          // Dashboard HTTP port
const MQTT_TCP_PORT = 1883;      // MQTT broker TCP port
const DEVICE_TIMEOUT = 60000;    // Device offline timeout (ms)
```

### Pin Customization

**Sensor** - Edit `ESP32-Sensor/include/config.h`:
```cpp
#define SDA_PIN 11               // I²C Data
#define SCL_PIN 12               // I²C Clock
#define NEOPIXEL_WIFI 45         // Status LED
#define RESET_BUTTON_PIN 0       // Boot button
```

**Actuator** - Edit `ESP32-Actuator/include/config.h`:
```cpp
#define NEOPIXEL_WIFI 45         // Status LED
#define RESET_BUTTON_PIN 0       // Boot button
```

GPIO output pins in `ESP32-Actuator/src/main.cpp`:
```cpp
const uint8_t gpioOutputPins[8] = {
  5, 6, 7, 8,      // Channels 1-4
  9, 10, 21, 38    // Channels 5-8
};
```

---

## 📡 MQTT Communication

### Topics Structure

#### Sensor → Server (Publishing)
```
devices/{deviceId}/telemetry
```

**Payload Example:**
```json
{
  "temperature": 24.5,
  "humidity": 65.2,
  "uptime": 123456,
  "wifiRSSI": -45,
  "mqttConnected": true,
  "freeHeap": 234567
}
```

#### Server → Actuator (Publishing)
```
device/{deviceId}/gpio/set
```

**Payload Example:**
```json
{
  "gpio": 1,
  "state": 1
}
```

Where:
- `gpio`: Channel number (1-8)
- `state`: 0 (OFF) or 1 (ON)

#### Actuator → Server (Subscribing)
The actuator subscribes to:
```
device/{deviceId}/gpio/set
```

### Quality of Service (QoS)
- **QoS 0** (At most once) - Default for all messages
- No message persistence
- Lightweight for real-time data

### Message Retention
- **No retention** - Messages are not stored by broker
- Devices receive only live messages
- Subscribe before publishing to receive messages

---

## 🖥️ Web Dashboard

### Dashboard Features

#### 1. Dashboard Tab
- **Stats Cards**: Total devices, online count, sensor count, automation count
- **Device Grid**: Live view of all connected devices
- **Refresh Button**: Manual device list refresh

#### 2. Device Fleet Tab
- **Search Bar**: Filter devices by name/ID
- **Device Cards**: 
  - Device type (sensor/actuator)
  - Online/offline status
  - IP address and WiFi signal
  - Temperature and humidity (sensors only)
  - Last seen timestamp

#### 3. Gesture Control Tab
- **Camera Feed**: Live video with hand tracking overlay
- **Gesture List**: 5 supported gestures with icons
- **Gesture Rules**: Map gestures to GPIO actions
- **Rule Management**: Create, edit, delete rules

Supported Gestures:
- 👋 **Open Palm** - All fingers extended
- ✊ **Fist** - All fingers closed
- ✌️ **Peace Sign** - Two fingers extended
- 👍 **Thumbs Up** - Thumb extended
- 👉 **Pointing** - Index finger extended

#### 4. Automation Tab
- **Rule Creator**: Define sensor thresholds
- **Condition Builder**: Temperature/humidity triggers
- **Action Selector**: GPIO channel and state
- **Rule List**: Active automation rules

Example Rule:
```
IF temperature > 30°C
THEN set GPIO 1 to ON (turn on fan)
```

#### 5. Event Logs Tab
- **Real-time Events**: System-wide event stream
- **Event Types**:
  - 🟢 Success (green)
  - 🔵 Info (blue)
  - 🟠 Warning (yellow)
  - 🔴 Error (red)
- **Timestamps**: ISO 8601 format
- **Event Details**: Expandable JSON payload
- **Clear Logs**: Reset event history

#### 6. MQTT Docs Tab
- MQTT topic reference
- Payload schemas
- Example messages
- Integration guide

### API Endpoints

#### GET /api/devices
Returns list of all registered devices.

**Response:**
```json
{
  "count": 2,
  "devices": [
    {
      "id": "ESP32-IOT-SENSOR-AB12",
      "type": "sensor",
      "online": true,
      "lastSeen": 1640000000000,
      "telemetry": {
        "temperature": 24.5,
        "humidity": 65.2
      }
    }
  ]
}
```

#### GET /api/server-ip
Returns server's local IP address.

**Response:**
```json
{
  "ip": "192.168.1.100"
}
```

#### POST /api/gpio/control
Send GPIO command to actuator.

**Request:**
```json
{
  "deviceId": "ESP32-IOT-ACTUATOR-CD34",
  "gpio": 1,
  "state": 1
}
```

**Response:**
```json
{
  "success": true,
  "message": "GPIO 1 set to 1 on ESP32-IOT-ACTUATOR-CD34"
}
```

#### GET /api/health
Server health check.

**Response:**
```json
{
  "status": "ok",
  "uptime": 123456,
  "mqttClients": 2,
  "httpPort": 3000,
  "mqttPort": 1883
}
```

---

## 👋 Gesture Control

### How It Works

1. **MediaPipe Hands** - Google's ML model detects 21 hand landmarks
2. **Gesture Recognition** - JavaScript analyzes landmark positions
3. **Confidence Filtering** - Only gestures with >85% confidence
4. **Cooldown Timer** - 2-second debounce to prevent spam
5. **MQTT Publishing** - Gesture triggers GPIO command
6. **Actuator Response** - Device executes GPIO action

### Creating Gesture Rules

1. **Go to Gesture Control Tab**
2. **Click "Start Camera"** - Allow camera access
3. **Scroll to "Gesture Rules"**
4. **Click "+ Add Rule"**
5. **Configure Rule**:
   - Select Gesture: Choose from dropdown
   - Select Device: Choose actuator device
   - Select GPIO: Channel 1-8
   - Select Action: ON or OFF
6. **Click "Create Rule"**
7. **Test Gesture** - Perform gesture in front of camera

### Gesture Detection Tips

- 📍 **Distance**: 30-60cm from camera
- 💡 **Lighting**: Good, even lighting
- 🖐️ **Hand Position**: Palm facing camera
- ⏱️ **Hold Time**: Hold gesture for 1 second
- 🎯 **Confidence**: Green outline = high confidence

---

## ⚡ Automation

### Creating Automation Rules

1. **Go to Automation Tab**
2. **Click "+ Add Rule"**
3. **Configure Trigger**:
   - Select Sensor: Choose sensor device
   - Select Condition:
     - Temperature > X
     - Temperature < X
     - Humidity > X
     - Humidity < X
4. **Configure Action**:
   - Select Actuator: Choose actuator device
   - Select GPIO: Channel 1-8
   - Select State: ON or OFF
5. **Click "Create Rule"**

### Example Automation Rules

**Climate Control:**
```
IF temperature > 28°C
THEN set GPIO 1 to ON (turn on fan)
```

**Humidity Management:**
```
IF humidity < 40%
THEN set GPIO 2 to ON (turn on humidifier)
```

**Combined Rules:**
```
IF temperature > 30°C
THEN set GPIO 1 to ON (fan)
AND  set GPIO 3 to OFF (heater)
```

### Rule Execution

- **Evaluation Interval**: Every time sensor publishes telemetry (5 seconds)
- **Debouncing**: Rules execute only on condition change (prevent spam)
- **State Tracking**: System remembers last trigger state
- **Logging**: All rule executions logged to events

---

## 🔧 Development

### FreeRTOS Task Architecture

#### Sensor Device (3 Tasks)

**TaskSensor** (Priority: 2)
- Reads DHT20 sensor every 1 second
- Uses I²C mutex for thread-safe access
- Puts data in telemetry queue
- Handles sensor errors and retries

**TaskMQTT** (Priority: 2)
- Dequeues telemetry data
- Publishes to MQTT broker
- Handles reconnection logic
- Sets connection event bits

**TaskUI** (Priority: 1)
- Updates NeoPixel LED color
- Reads connection event bits
- 500ms update interval
- Visual status feedback

#### Actuator Device (3 Tasks)

**TaskActuator** (Priority: 2)
- Dequeues commands from command queue
- Parses JSON payloads
- Sets GPIO pin states
- Error handling and logging

**TaskMQTT** (Priority: 2)
- Subscribes to command topics
- Enqueues incoming commands
- Publishes status telemetry
- Handles reconnection logic

**TaskUI** (Priority: 1)
- Updates NeoPixel LED color
- 500ms update interval
- Visual status feedback

### Synchronization Primitives

**Mutexes:**
- `i2cMutex` (Sensor) - Protects I²C bus access

**Queues:**
- `telemetryQueue` (Sensor) - Sensor → MQTT task
- `commandQueue` (Actuator) - MQTT → Actuator task

**Event Groups:**
- `connectionEvents` - WiFi and MQTT status bits

### Adding New Sensor

1. **Add I²C Device**:
   ```cpp
   // In main.cpp setup()
   MySensor sensor;
   sensor.begin(0x50);  // I²C address
   ```

2. **Modify TaskSensor**:
   ```cpp
   // In tasks.cpp
   if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100))) {
     float data = sensor.readData();
     xSemaphoreGive(i2cMutex);
     
     // Add to telemetry
     telemetryData.myData = data;
   }
   ```

3. **Update MQTT Payload**:
   ```cpp
   // In mqtt_handler.cpp
   doc["myData"] = telemetry.myData;
   ```

### Adding GPIO Actuator

1. **Define Pin**:
   ```cpp
   // In config.h
   #define MY_ACTUATOR_PIN 15
   ```

2. **Initialize Pin**:
   ```cpp
   // In main.cpp setup()
   pinMode(MY_ACTUATOR_PIN, OUTPUT);
   ```

3. **Add Command Handler**:
   ```cpp
   // In tasks.cpp TaskActuator
   if (cmd.type == "my_actuator") {
     digitalWrite(MY_ACTUATOR_PIN, cmd.state);
   }
   ```

### Building Custom Dashboard Widget

1. **Add HTML**:
   ```html
   <!-- In index.html -->
   <div class="card">
     <h3 class="card-title">My Widget</h3>
     <div id="myWidget"></div>
   </div>
   ```

2. **Add JavaScript**:
   ```javascript
   // In ui.js
   function updateMyWidget(data) {
     const widget = document.getElementById('myWidget');
     widget.textContent = `Value: ${data.myValue}`;
   }
   ```

3. **Subscribe to MQTT**:
   ```javascript
   // In mqtt.js
   mqttClient.on('message', (topic, payload) => {
     if (topic.includes('telemetry')) {
       const data = JSON.parse(payload);
       updateMyWidget(data);
     }
   });
   ```

---

## 🐛 Troubleshooting

### ESP32 Issues

**NeoPixel stays orange/blue:**
- ✅ Check WiFi SSID and password
- ✅ Verify MQTT server IP is correct
- ✅ Ensure MQTT server is running
- ✅ Check firewall isn't blocking port 1883

**Sensor readings are NaN:**
- ✅ Check DHT20 I²C wiring (SDA, SCL)
- ✅ Verify I²C address with diagnostics
- ✅ Ensure 3.3V power to sensor
- ✅ Add pull-up resistors if needed (4.7kΩ)

**WiFi won't connect:**
- ✅ Verify 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- ✅ Check WiFi password is correct
- ✅ Ensure WiFi network is within range
- ✅ Try factory reset (hold boot button 3s)

**MQTT disconnects frequently:**
- ✅ Check WiFi signal strength (RSSI)
- ✅ Verify MQTT server is stable
- ✅ Increase MQTT keepalive interval
- ✅ Check network stability

**Upload fails:**
- ✅ Hold Boot button while uploading
- ✅ Check USB cable quality (data, not charge-only)
- ✅ Verify correct COM port in platformio.ini
- ✅ Close Serial Monitor before uploading
- ✅ Try different USB port

### Web Server Issues

**Server won't start:**
```bash
# Check if port 3000 is in use
netstat -an | findstr 3000  # Windows
lsof -i :3000               # Mac/Linux

# Kill process using port
# Windows: Task Manager
# Mac/Linux: kill -9 <PID>
```

**Devices don't appear:**
- ✅ Check MQTT connection in browser console
- ✅ Verify devices are publishing telemetry
- ✅ Clear browser cache (Ctrl+Shift+Delete)
- ✅ Check firewall rules

**Gesture recognition not working:**
- ✅ Allow camera permissions in browser
- ✅ Use HTTPS or localhost (WebRTC requirement)
- ✅ Check console for MediaPipe errors
- ✅ Try different browser (Chrome recommended)

### Network Issues

**Can't find computer's IP address:**
```bash
# Windows
ipconfig

# Mac
ifconfig

# Linux
ip addr show
hostname -I
```

**ESP32 can't reach MQTT broker:**
- ✅ Ping test: `ping <your-ip>` from another device
- ✅ Check firewall (allow port 1883)
- ✅ Verify devices on same subnet
- ✅ Try connecting from phone browser to `http://<ip>:3000`

---

## 📚 Additional Resources

### Documentation
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [PlatformIO Docs](https://docs.platformio.org/)
- [Aedes MQTT Broker](https://github.com/moscajs/aedes)
- [MediaPipe Hands](https://google.github.io/mediapipe/solutions/hands.html)

### Libraries Used

**ESP32 Firmware:**
- [PubSubClient](https://github.com/knolleary/pubsubclient) - MQTT client
- [ArduinoJson](https://arduinojson.org/) - JSON serialization
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - LED control
- [DHT20](https://github.com/RobTillaart/DHT20) - Temperature/humidity sensor

**Node.js Server:**
- [Express](https://expressjs.com/) - Web framework
- [Aedes](https://github.com/moscajs/aedes) - MQTT broker
- [ws](https://github.com/websockets/ws) - WebSocket server
- [websocket-stream](https://github.com/maxogden/websocket-stream) - MQTT over WebSocket

**Web Dashboard:**
- [MQTT.js](https://github.com/mqttjs/MQTT.js) - MQTT client for browser
- [MediaPipe](https://google.github.io/mediapipe/) - Hand gesture recognition

---

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Commit changes**: `git commit -m 'Add amazing feature'`
4. **Push to branch**: `git push origin feature/amazing-feature`
5. **Open a Pull Request**

### Code Style
- **C++**: Follow Arduino style guide
- **JavaScript**: Use ES6+ features, camelCase
- **Comments**: Document complex logic
- **Testing**: Test on real hardware before PR

---

## 📝 License

This project is licensed under the **MIT License** - see the LICENSE file for details.

---

## 👥 Authors

- **Hoai Nhan** - *Initial work* - [leonathn](https://github.com/leonathn)

---

## 🙏 Acknowledgments

- **Espressif Systems** - ESP32-S3 hardware
- **Google MediaPipe** - Hand gesture recognition
- **Arduino Community** - Libraries and support
- **MQTT.org** - Protocol specification
- **PlatformIO** - Development platform

---

## 📊 Project Stats

- **Lines of Code**: ~5,000+
- **Files**: 50+
- **Languages**: C++, JavaScript, HTML, CSS
- **Development Time**: Final Project
- **Purpose**: Embedded Systems Course

---

## 🔮 Future Enhancements

- [ ] **HTTPS Support** - Secure web dashboard
- [ ] **User Authentication** - Login system
- [ ] **Database Integration** - Historical data storage (InfluxDB)
- [ ] **Grafana Dashboards** - Advanced visualization
- [ ] **Voice Control** - Alexa/Google Home integration
- [ ] **Mobile App** - React Native companion app
- [ ] **OTA Updates** - Over-the-air firmware updates
- [ ] **Multi-room Support** - Zone-based automation
- [ ] **Energy Monitoring** - Power consumption tracking
- [ ] **Weather Integration** - External weather data
- [ ] **Docker Support** - Containerized deployment
- [ ] **Cloud Sync** - Optional cloud backup

---

## 📞 Support

For questions, issues, or feature requests:
- **GitHub Issues**: [Open an issue](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS/issues)
- **Email**: Contact via GitHub profile

---

<div align="center">

**Made with ❤️ for the IoT Community**

⭐ **Star this repo** if you find it useful!

[🏠 Home](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS) • 
[📖 Docs](#-table-of-contents) • 
[🐛 Report Bug](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS/issues) • 
[✨ Request Feature](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS/issues)

</div>
