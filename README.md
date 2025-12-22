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
- [Software Requirements](#-software-requirements)
- [Installation & Setup](#-installation--setup)
- [Quick Start Guide](#-quick-start-guide)
- [Device Configuration](#-device-configuration)
- [Web Dashboard Usage](#-web-dashboard-usage)
- [MQTT Communication](#-mqtt-communication)
- [Automation & Gesture Control](#-automation--gesture-control)
- [Troubleshooting](#-troubleshooting)
- [License](#-license)

---

## 🎯 Overview

This is a **production-ready, enterprise-grade IoT system** built with ESP32-S3 microcontrollers, featuring a complete ecosystem for environmental monitoring and remote device control. The system integrates hardware, firmware, and web technologies to provide a comprehensive IoT solution.

### What Makes This Special?

- **Zero-Touch Configuration**: Captive portal setup with WiFi manager - no hardcoded credentials needed
- **Local MQTT Broker**: Private, secure communication without cloud dependency using built-in Aedes MQTT broker
- **Dual Device Architecture**: Separate optimized firmware for sensors (DHT20) and actuators (8-channel GPIO)
- **Real-time Dashboard**: Modern web interface with live monitoring, device management, and control
- **AI Gesture Control**: MediaPipe-powered hand gesture recognition for touchless device control
- **FreeRTOS Multithreading**: Efficient task management with proper synchronization primitives
- **Automation Engine**: Rule-based automation with sensor thresholds and automatic actions

### Technology Stack

| Component | Technology |
|-----------|-----------|
| **Microcontroller** | ESP32-S3 (dual-core Xtensa LX7, 240MHz) |
| **Framework** | Arduino Core for ESP32 |
| **RTOS** | FreeRTOS (native ESP-IDF) |
| **Communication** | WiFi (802.11 b/g/n), MQTT 3.1.1 |
| **Sensors** | DHT20 (I²C temperature & humidity) |
| **Server** | Node.js, Express, Aedes MQTT Broker |
| **Frontend** | HTML5, CSS3, JavaScript (Vanilla) |
| **AI** | MediaPipe Hands (Google ML) |
| **Indicators** | WS2812B NeoPixel RGB LED |

### Use Cases

- **Home Automation**: Temperature/humidity monitoring with automated climate control
- **Industrial IoT**: Remote sensor monitoring with actuator control via MQTT
- **Smart Agriculture**: Environmental monitoring and automated irrigation systems
- **Education**: Learn IoT, MQTT, FreeRTOS, embedded systems, and web development
- **Rapid Prototyping**: Quick development and testing of IoT applications

---

## 🏗️ System Architecture

The system consists of three main components working together seamlessly:

```
┌───────────────────────────────────────────────────────────────────┐
│                         Web Dashboard                             │
│  ┌───────────┐ ┌──────────┐ ┌──────────┐ ┌──────────────────┐     │
│  │ Dashboard │ │  Devices │ │ Gestures │ │   Automation     │     │
│  │  Monitor  │ │  Fleet   │ │ Control  │ │  & Event Logs    │     │
│  └───────────┘ └──────────┘ └──────────┘ └──────────────────┘     │
│       │              │             │              │               │
│       └──────────────┴─────────────┴──────────────┘               │
│                           │                                       │
│                ┌──────────▼──────────┐                            │
│                │   Node.js Server    │                            │
│                │  ┌────────────────┐ │                            │
│                │  │ Express HTTP   │ │  Port 3000                 │
│                │  │ Aedes MQTT     │ │  Port 1883                 │
│                │  │ WebSocket      │ │  ws://server:3000          │
│                │  │ Automation     │ │                            │
│                │  └────────────────┘ │                            │
│                └──────────┬──────────┘                            │
└───────────────────────────┼───────────────────────────────────────┘
                            │ MQTT Protocol
                            │
              ┌─────────────┴────────────┐
              │                          │
     ┌────────▼────────┐         ┌───────▼────────┐
     │  ESP32-Sensor   │         │ ESP32-Actuator │
     │  (Monitoring)   │         │   (Control)    │
     │                 │         │                │
     │ ┌─────────────┐ │         │ ┌────────────┐ │
     │ │ DHT20       │ │         │ │ 8-Channel  │ │
     │ │ I²C Sensor  │ │         │ │ GPIO Ctrl  │ │
     │ │ Temp/Humid  │ │         │ │ Relay Out  │ │
     │ └─────────────┘ │         │ └────────────┘ │
     │                 │         │                │
     │ ┌─────────────┐ │         │ ┌────────────┐ │
     │ │ NeoPixel    │ │         │ │ NeoPixel   │ │
     │ │ Status LED  │ │         │ │ Status LED │ │
     │ └─────────────┘ │         │ └────────────┘ │
     │                 │         │                │
     │ ┌─────────────┐ │         │ ┌────────────┐ │
     │ │ WiFi Portal │ │         │ │WiFi Portal │ │
     │ │ Captive AP  │ │         │ │Captive AP  │ │
     │ └─────────────┘ │         │ └────────────┘ │
     │                 │         │                │
     │  FreeRTOS       │         │  FreeRTOS      │
     │  3 Tasks        │         │  3 Tasks       │
     │  Mutex + Queue  │         │  Queue         │
     └─────────────────┘         └────────────────┘
```

### Communication Flow

1. **ESP32 Sensors** → Publish telemetry data (temperature, humidity, WiFi signal) to MQTT broker
2. **Node.js Server** → Receives sensor data, evaluates automation rules, stores device states
3. **Web Dashboard** → Displays real-time data via WebSocket, sends control commands
4. **Automation Engine** → Triggers actuator commands based on sensor thresholds
5. **Gesture Recognition** → Camera input → MediaPipe ML → GPIO commands → MQTT
6. **ESP32 Actuators** → Subscribe to GPIO commands, control physical devices

---

## ✨ Key Features

### ESP32 Sensor Node
- ✅ **DHT20 I²C Sensor** - Accurate temperature (±0.3°C) and humidity (±3% RH) monitoring
- ✅ **FreeRTOS Multitasking** - Three concurrent tasks (sensor, MQTT, UI) with mutex synchronization
- ✅ **MQTT Telemetry** - Real-time data publishing every 5 seconds with QoS support
- ✅ **WiFi Manager** - Captive portal configuration via AP mode (SSID: ESP32-IOT-SENSOR-XXXX)
- ✅ **NeoPixel Status LED** - 5-color visual feedback (orange/blue/yellow/green/red)
- ✅ **System Diagnostics** - Built-in health checks, I²C bus scanning, error reporting
- ✅ **NVS Storage** - Non-volatile configuration persistence across reboots
- ✅ **Factory Reset** - Hold BOOT button for 3 seconds to clear all settings
- ✅ **mDNS Support** - Device discoverable as `<device_id>.local` on network

### ESP32 Actuator Node
- ✅ **8-Channel GPIO Control** - Independent control of relays, LEDs, motors, solenoids
- ✅ **MQTT Command Subscription** - Receives commands from server/dashboard via MQTT
- ✅ **FreeRTOS Command Queue** - Buffered command processing with overflow protection
- ✅ **Specialized Firmware** - Optimized for actuator operations (no sensor overhead)
- ✅ **WiFi Manager** - Same captive portal as sensor for consistent setup
- ✅ **NeoPixel Status LED** - Connection status visualization
- ✅ **Telemetry Publishing** - Real-time status updates to MQTT broker
- ✅ **Factory Reset** - 3-second BOOT button press

### Node.js Web Server
- ✅ **Built-in MQTT Broker** - Aedes broker with TCP (1883) and WebSocket (3000) support
- ✅ **Real-time Dashboard** - Live device monitoring with 1-second refresh rate
- ✅ **Multi-Device Management** - Track and control multiple ESP32 devices simultaneously
- ✅ **Device Fleet View** - Comprehensive device list with search/filter capabilities
- ✅ **GPIO Control Panel** - Remote control of actuator GPIO pins via toggle switches
- ✅ **Event Logging System** - Comprehensive activity logs with color-coded event types
- ✅ **Automation Rules** - IF-THEN logic with sensor thresholds and automatic actions
- ✅ **Auto-Toggle Support** - Automatic state reversal when conditions change
- ✅ **Responsive Design** - Modern cyberpunk-themed UI with dark mode
- ✅ **Zero Configuration** - Automatic device discovery and registration

### AI Gesture Recognition
- ✅ **5 Hand Gestures** - Open palm, fist, peace sign, thumbs up, pointing finger
- ✅ **MediaPipe Hands** - Google's ML model with 21-point hand tracking
- ✅ **WebRTC Camera Access** - Browser-based camera integration
- ✅ **Gesture-to-GPIO Mapping** - Assign any gesture to any actuator GPIO action
- ✅ **Confidence Threshold** - 85% minimum confidence for reliable detection
- ✅ **Debounce Timer** - 2-second cooldown between gesture triggers
- ✅ **Real-time Feedback** - Live hand tracking overlay on camera feed

---

## 📂 Project Structure

```
Final_Project_Embedded_Systems/
│
├── 📁 ESP32-Sensor/                   # Sensor Firmware (DHT20)
│   ├── platformio.ini                # Build configuration
│   ├── README.md                     # Sensor-specific documentation
│   │
│   ├── 📁 include/                   # Header files
│   │   ├── config.h                 # Pin definitions & constants
│   │   ├── types.h                  # Data structures & enums
│   │   ├── globals.h                # Global variables & objects
│   │   ├── config_manager.h         # NVS storage functions
│   │   ├── wifi_manager.h           # WiFi AP/STA management
│   │   ├── mqtt_handler.h           # MQTT client functions
│   │   ├── neopixel_handler.h       # LED status indicator
│   │   ├── diagnostics.h            # System health checks
│   │   ├── web_server.h             # Captive portal web UI
│   │   └── tasks.h                  # FreeRTOS task definitions
│   │
│   ├── 📁 src/                       # Source code
│   │   ├── main.cpp                 # Main entry point
│   │   ├── config_manager.cpp       # Configuration persistence
│   │   ├── wifi_manager.cpp         # WiFi connection handling
│   │   ├── mqtt_handler.cpp         # MQTT messaging
│   │   ├── neopixel_handler.cpp     # LED control
│   │   ├── diagnostics.cpp          # Diagnostics implementation
│   │   ├── web_server.cpp           # Web UI & API handlers
│   │   └── tasks.cpp                # FreeRTOS task implementations
│   │
│   └── 📁 boards/
│       └── yolo_uno.json            # Custom board configuration
│
├── 📁 ESP32-Actuator/                 # Actuator Firmware (GPIO Control)
│   ├── platformio.ini                # Build configuration
│   ├── README.md                     # Actuator-specific documentation
│   │
│   ├── 📁 include/                   # Header files
│   │   ├── config.h                 # Pin definitions & constants
│   │   ├── types.h                  # Data structures & enums
│   │   ├── globals.h                # Global variables & objects
│   │   ├── config_manager.h         # NVS storage functions
│   │   ├── wifi_manager.h           # WiFi AP/STA management
│   │   ├── mqtt_handler.h           # MQTT command subscription
│   │   ├── neopixel_handler.h       # LED status indicator
│   │   ├── web_server.h             # Captive portal web UI
│   │   └── tasks.h                  # FreeRTOS task definitions
│   │
│   └── 📁 src/                       # Source code
│       ├── main.cpp                 # Main entry point
│       ├── config_manager.cpp       # Configuration persistence
│       ├── wifi_manager.cpp         # WiFi connection handling
│       ├── mqtt_handler.cpp         # MQTT command processing
│       ├── neopixel_handler.cpp     # LED control
│       ├── web_server.cpp           # Web UI & API handlers
│       └── tasks.cpp                # FreeRTOS task implementations
│
├── 📁 Web-Server/                     # Node.js Server & Dashboard
│   ├── README.md                     # Server documentation
│   │
│   └── 📁 server/
│       ├── package.json             # Node.js dependencies
│       ├── server.js                # Main server (Express + Aedes)
│       │
│       └── 📁 public/                # Web Dashboard Frontend
│           ├── index.html           # Main HTML page
│           │
│           ├── 📁 css/              # Stylesheets
│           │   ├── variables.css   # Design tokens
│           │   ├── layout.css      # Page layout & grid
│           │   └── components.css  # UI components
│           │
│           └── 📁 js/               # JavaScript modules
│               ├── mqtt.js         # MQTT WebSocket client
│               ├── devices.js      # Device management
│               ├── gestures.js     # AI gesture recognition
│               ├── automation.js   # Automation rules engine
│               ├── events.js       # Event logging system
│               └── ui.js           # UI controls & navigation
│
└── 📄 README.md                       # This file (Project overview)
```

---

## 🔧 Hardware Requirements

### ESP32 Sensor Node

| Component | Quantity | Specifications | Notes |
|-----------|----------|----------------|-------|
| ESP32-S3 DevKit | 1 | Dual-core 240MHz, WiFi | Main microcontroller |
| DHT20 Sensor | 1 | I²C interface | Temperature & humidity |
| WS2812B NeoPixel | 1 | RGB LED | Built-in on GPIO 45 |
| USB-C Cable | 1 | For programming | Power & data |
| Breadboard | 1 | For prototyping | Optional |
| Jumper Wires | 8 | Male-to-male | For connections |

**Sensor Pin Assignments:**
- **DHT20 SDA**: GPIO 11 (I²C Data)
- **DHT20 SCL**: GPIO 12 (I²C Clock)
- **NeoPixel**: GPIO 45 (Status LED)
- **Reset**: GPIO 0 (BOOT button)

### ESP32 Actuator Node

| Component | Quantity | Specifications | Notes |
|-----------|----------|----------------|-------|
| ESP32-S3 DevKit | 1 | Dual-core 240MHz, WiFi | Main microcontroller |
| WS2812B NeoPixel | 1 | RGB LED | Built-in on GPIO 45 |
| Relay Module | 1-8 | 5V trigger, optocoupler | Optional for switching |
| USB-C Cable | 1 | For programming | Power & data |
| Power Supply | 1 | 5V, 2A+ | For relays/loads |
| Breadboard | 1 | For prototyping | Optional |
| Jumper Wires | 12 | Male-to-male | For connections |

**Actuator GPIO Pin Mapping (8 Channels):**
- **Channel 1**: GPIO 5
- **Channel 2**: GPIO 6
- **Channel 3**: GPIO 7
- **Channel 4**: GPIO 8
- **Channel 5**: GPIO 9
- **Channel 6**: GPIO 10
- **Channel 7**: GPIO 21
- **Channel 8**: GPIO 38
- **NeoPixel**: GPIO 45 (Status LED)
- **Reset**: GPIO 0 (BOOT button)

### Server/Dashboard Requirements

| Component | Requirement |
|-----------|-------------|
| Computer | Windows/Mac/Linux |
| RAM | 4GB minimum |
| Storage | 500MB free space |
| Webcam | For gesture control (optional) |
| Browser | Chrome/Edge/Firefox (Chrome recommended) |

### Network Requirements

- WiFi router with 2.4 GHz support (ESP32 compatible)
- All devices on same local network
- Open ports: 1883 (MQTT), 3000 (HTTP/WebSocket)
- Static or DHCP IP for server (recommended)

---

## 💾 Software Requirements

### For ESP32 Development

| Software | Version | Purpose |
|----------|---------|---------|
| Visual Studio Code | Latest | IDE for development |
| PlatformIO IDE | Latest | ESP32 build system |
| Python | 3.7+ | PlatformIO dependency |
| USB-to-Serial Driver | CP210x/CH340 | ESP32 communication |

**Required Arduino Libraries (auto-installed by PlatformIO):**
- `WiFi.h` (built-in)
- `PubSubClient` - MQTT client
- `ArduinoJson` - JSON parsing
- `DHT20` - DHT20 sensor library
- `Adafruit_NeoPixel` - LED control
- `ESPAsyncWebServer` - Web server
- `Preferences` - NVS storage

### For Node.js Server

| Software | Version | Purpose |
|----------|---------|---------|
| Node.js | 16.0.0+ | JavaScript runtime |
| npm | 8.x+ | Package manager |

**Node.js Dependencies (auto-installed):**
```json
{
  "express": "^4.18.2",        // Web server
  "aedes": "^0.50.0",          // MQTT broker
  "websocket-stream": "^5.5.2", // WebSocket support
  "mqtt": "^5.3.0"             // MQTT client
}
```

---

## 🚀 Installation & Setup

### Step 1: Install Development Tools

#### 1.1 Install Visual Studio Code
1. Download from: https://code.visualstudio.com/
2. Install for your operating system
3. Launch VS Code

#### 1.2 Install PlatformIO IDE Extension
1. Open VS Code
2. Press `Ctrl+Shift+X` (or `Cmd+Shift+X` on Mac)
3. Search for **"PlatformIO IDE"**
4. Click **Install**
5. Wait for installation to complete
6. Restart VS Code

#### 1.3 Install Node.js
1. Download from: https://nodejs.org/
2. Choose **LTS version** (16.x or higher)
3. Run installer with default options
4. Verify installation:
   ```bash
   node --version
   npm --version
   ```

### Step 2: Clone the Repository

```bash
# Clone the project
git clone https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS.git

# Navigate to project directory
cd FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS
```

### Step 3: Setup ESP32 Sensor

```bash
# Navigate to sensor firmware
cd ESP32-Sensor
```

1. **Connect ESP32-S3 to computer** via USB-C cable
2. **Identify COM/Serial Port**:
   - **Windows**: Open Device Manager → Ports (COM & LPT) → Look for "USB-SERIAL CH340" or "CP210x"
   - **Mac**: Run `ls /dev/cu.*` in terminal
   - **Linux**: Run `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`

3. **Update PlatformIO Configuration**:
   - Open `platformio.ini`
   - Update the upload port:
     ```ini
     upload_port = COM3        ; Windows (change to your port)
     ; upload_port = /dev/cu.usbserial-0001  ; Mac
     ; upload_port = /dev/ttyUSB0            ; Linux
     monitor_port = COM3       ; Same as upload_port
     ```

4. **Build and Upload Firmware**:
   - In VS Code, click the PlatformIO icon (alien head) on the left sidebar
   - Under **PROJECT TASKS** → **esp32s3** → click **Upload and Monitor**
   - Wait for compilation and upload to complete
   - Monitor serial output to verify successful boot

### Step 4: Setup ESP32 Actuator

```bash
# Navigate to actuator firmware
cd ../ESP32-Actuator
```

1. **Connect second ESP32-S3** (or disconnect sensor and use same device)
2. **Update PlatformIO Configuration** (same as sensor setup)
3. **Build and Upload**:
   - Click **Upload and Monitor** in PlatformIO
   - Verify successful boot in serial monitor

### Step 5: Setup Web Server

```bash
# Navigate to server directory
cd ../Web-Server/server

# Install dependencies
npm install

# Start the server
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

### Step 6: Find Your Server IP Address

ESP32 devices need your computer's **local network IP address** (not `localhost`).

**Windows:**
```powershell
ipconfig
```
Look for **IPv4 Address** under your active WiFi/Ethernet adapter (e.g., `192.168.1.100`)

**macOS:**
```bash
ifconfig | grep "inet " | grep -v 127.0.0.1
```

**Linux:**
```bash
ip addr show | grep "inet " | grep -v 127.0.0.1
# or
hostname -I
```

**Example IP:** `192.168.1.100` ← You'll need this for ESP32 configuration

---

## 🎮 Quick Start Guide

### Device Configuration Process

Both ESP32 Sensor and Actuator use the same WiFi configuration workflow:

#### Step 1: Power On ESP32

1. Connect ESP32 to USB power
2. **NeoPixel LED Status:**
   - **Orange (booting)** → System initializing
   - **Blue (AP mode)** → No configuration found, broadcasting WiFi

#### Step 2: Connect to Device WiFi

3. On your smartphone or laptop, open WiFi settings
4. Look for WiFi network:
   - **Sensor**: `ESP32-IOT-SENSOR-XXXX` (XXXX = unique ID)
   - **Actuator**: `ESP32-IOT-ACTUATOR-XXXX`
5. Connect using password: **`12345678`**

#### Step 3: Configure Device

6. **Captive portal should open automatically**
   - If not, manually browse to: **`http://192.168.4.1`**
   
7. **Fill in configuration form:**

   | Field | Value | Example |
   |-------|-------|---------|
   | **WiFi SSID** | Your home WiFi name | `MyHomeNetwork` |
   | **WiFi Password** | Your WiFi password | `MySecurePass123` |
   | **MQTT Server** | Server computer IP | `192.168.1.100` |
   | **MQTT Port** | MQTT broker port | `1883` |
   | **Pairing Token** | Optional security code | `mytoken123` |

8. Click **"Save Configuration"**

#### Step 4: Verify Connection

9. Device will automatically reboot and connect
10. **NeoPixel LED Status Indicators:**
    - **🟢 Green**: Fully operational (WiFi ✓ + MQTT ✓)
    - **🟡 Yellow**: WiFi connected, MQTT not connected
    - **🔴 Red**: Connection error (check credentials)

### Access the Dashboard

1. On a computer connected to the same WiFi network
2. Open web browser (Chrome recommended)
3. Navigate to: **`http://localhost:3000`**
   - Or use server IP: **`http://192.168.1.100:3000`**

4. **Verify devices are online:**
   - Dashboard should show device count
   - Device cards should appear in "Device Fleet" tab
   - Real-time telemetry should be visible (sensor data)

### Factory Reset (if needed)

To clear device configuration and return to AP mode:

1. Hold the **BOOT button** (GPIO 0) on ESP32
2. Keep holding for **3 seconds**
3. NeoPixel will blink red
4. Release button
5. Device clears configuration and reboots to AP mode (blue LED)
---

## 🎛️ Device Configuration

### NeoPixel LED Status Indicators

Both Sensor and Actuator use the same color coding for status feedback:

| Color | Status | Meaning | Action Required |
|-------|--------|---------|-----------------|
| 🟠 **Orange** | Booting | System initializing | Wait a few seconds |
| 🔵 **Blue** | AP Mode | Configuration needed | Connect to WiFi and configure |
| 🟡 **Yellow** | WiFi Only | WiFi ✓, MQTT ✗ | Check MQTT server IP/port |
| 🟢 **Green** | Fully Connected | WiFi ✓, MQTT ✓ | System operational |
| 🔴 **Red** | Error | Connection failed | Check credentials/network |

### Configuration Parameters

| Parameter | Description | Example | Notes |
|-----------|-------------|---------|-------|
| **WiFi SSID** | Your network name | `MyHomeNetwork` | Case-sensitive, 2.4 GHz only |
| **WiFi Password** | Network password | `MySecurePass123` | Min 8 characters |
| **MQTT Server** | Server IP address | `192.168.1.100` | Use local IP, not localhost |
| **MQTT Port** | Broker TCP port | `1883` | Default MQTT port |
| **Pairing Token** | Security code | `mytoken123` | Optional, any string |

### Advanced Configuration

#### Changing GPIO Pins (Actuator)

Edit `ESP32-Actuator/src/main.cpp`:

```cpp
// Modify GPIO output pins array
const uint8_t gpioOutputPins[8] = {
  5, 6, 7, 8,      // Channels 1-4
  9, 10, 21, 38    // Channels 5-8
};
```

#### Changing I²C Pins (Sensor)

Edit `ESP32-Sensor/include/config.h`:

```cpp
#define SDA_PIN 11   // I²C Data line
#define SCL_PIN 12   // I²C Clock line
```

#### Modifying MQTT Topics

Edit respective `include/config.h` files:

```cpp
// Telemetry publishing topic (sensor)
#define MQTT_TOPIC_TELEMETRY "devices/%s/telemetry"

// Command subscription topic (actuator)
#define MQTT_TOPIC_GPIO "device/%s/gpio/set"
```

---

## 🖥️ Web Dashboard Usage

### Dashboard Overview

The web interface consists of **6 main tabs**, each serving specific functions:

#### 1. 📊 Dashboard Tab (Home)

**Quick overview of your IoT fleet:**

- **Stats Cards**: 
  - Total Devices (all registered)
  - Online Devices (currently connected)
  - Sensors Count
  - Active Automations Count
  
- **Device Preview**:
  - Shows up to 2 connected devices
  - Real-time telemetry data
  - Quick GPIO controls (3 switches per actuator)
  - "Control More" button for full 8-channel access

- **Refresh Rate**: Live updates every 1 second

#### 2. 🔧 Device Fleet Tab

**Comprehensive device management interface:**

- **Search Bar**: Filter devices by name or ID
- **Device Cards** display:
  - Device name and type badge (Sensor/Actuator)
  - Online/Offline status with timestamp
  - IP address and WiFi signal strength (RSSI)
  - Real-time telemetry (temperature, humidity for sensors)
  - GPIO toggle switches (8 pins for actuators)
  - System info (heap memory, uptime)
  - Last update timestamp

- **Device Types**:
  - **Sensors**: Display environmental data
  - **Actuators**: GPIO control with toggle switches
  - **Hybrid**: Both sensor readings and GPIO control

#### 3. 👋 Gesture Control Tab

**AI-powered hand gesture recognition for touchless control:**

**Supported Gestures:**
- ✋ **Open Palm** - All fingers extended
- ✊ **Fist** - All fingers closed
- ✌️ **Peace/Victory** - Two fingers (index + middle)
- 👍 **Thumbs Up** - Thumb extended upward
- ☝️ **Point Up** - Index finger extended

**Features:**
- Live camera feed with hand tracking overlay
- Gesture confidence display (must be >85%)
- Create gesture-to-GPIO mapping rules
- Enable/disable rules with toggle switches
- Edit and delete existing rules
- 2-second cooldown between triggers

**Tips for Best Results:**
- Use good lighting conditions
- Keep hand 30-60 cm from camera
- Show only one hand at a time
- Hold gesture steady for 1 second
- Allow browser camera access

#### 4. ⚡ Automation Tab

**Create intelligent automation rules with IF-THEN logic:**

**Rule Components:**

**Condition (IF):**
- Source Device: Select sensor device
- Parameter: temperature, humidity, or wifiRSSI
- Operator: >, <, >=, <=, ==
- Threshold: Numeric value

**Action (THEN):**
- Target Device: Select actuator device
- GPIO Pin: 1-8
- State: ON or OFF

**Auto-Toggle:** Automatically reverse action when condition is no longer met

**Example Automation Rules:**

```
Rule 1: Temperature Control
IF temperature > 30°C
THEN set actuator GPIO 1 to ON (turn on fan)
Auto-toggle: Yes (fan turns off when temp ≤ 30°C)

Rule 2: Humidity Control
IF humidity < 40%
THEN set actuator GPIO 2 to ON (humidifier)
Auto-toggle: Yes

Rule 3: WiFi Signal Alert
IF wifiRSSI < -70
THEN set actuator GPIO 3 to ON (warning LED)
Auto-toggle: No
```

**Rule Management:**
- Enable/disable with toggle switches
- Edit existing rules
- Delete unwanted rules
- Active rules run automatically in background

#### 5. 📋 Event Logs Tab

**System-wide activity monitoring:**

**Event Types:**
- 🔵 **Info**: General system events
- ✅ **Success**: Successful operations
- ⚠️ **Warning**: Important notifications
- ❌ **Error**: Error messages

**Features:**
- Timestamp for each event (ISO 8601 format)
- Color-coded event types
- Scrollable timeline
- Auto-scroll to newest events
- Export logs as JSON
- Clear all logs button

**Example Events:**
- Device connected/disconnected
- GPIO state changes
- Automation rule triggers
- Gesture detections
- MQTT connection status

#### 6. 📖 MQTT Docs Tab

**Complete API documentation and code examples:**

- **Connection Details**: Broker address, ports, protocols
- **Topic Structure**: Subscribe and publish topics
- **Payload Formats**: JSON message structures
- **ESP32 Arduino Examples**:
  - MQTT connection setup
  - Sensor data publishing
  - GPIO command handling
  - Complete actuator example
- **Python Client**: paho-mqtt example code
- **Integration Guide**: How to add custom devices

---

## 📡 MQTT Communication

### MQTT Broker Details

| Parameter | Value | Description |
|-----------|-------|-------------|
| **Protocol** | MQTT v3.1.1 | Standard MQTT protocol |
| **TCP Port** | 1883 | Standard MQTT port |
| **WebSocket Port** | 3000 | WebSocket MQTT (ws://server:3000) |
| **QoS** | 0 (At most once) | Fire-and-forget delivery |
| **Retained Messages** | No | Messages not persisted |
| **Authentication** | None | Open broker (local network only) |
| **Clean Session** | Yes | No session persistence |

### Topic Structure

#### Sensor → Server (Publishing)

**Device Registration:**
```
Topic: esp32/register

Payload:
{
  "id": "ESP32-IOT-SENSOR-AB12",
  "type": "sensor",
  "name": "Living Room Sensor",
  "ip": "192.168.1.150",
  "rssi": -45
}
```

**Telemetry Data:**
```
Topic: devices/{deviceId}/telemetry

Payload:
{
  "temperature": 24.5,
  "humidity": 65.2,
  "uptime": 123456,
  "wifiRSSI": -45,
  "mqttConnected": true,
  "freeHeap": 234567,
  "dataQuality": 95.0
}
```

#### Server → Actuator (Publishing)

**GPIO Control Command:**
```
Topic: device/{deviceId}/gpio/set

Payload:
{
  "gpio": 1,
  "state": 1
}
```

Where:
- `gpio`: Channel number (1-8)
- `state`: 0 (OFF/LOW) or 1 (ON/HIGH)

#### Actuator → Server (Subscribing)

Actuators subscribe to:
```
device/{deviceId}/gpio/set
```

### Message Examples

**Sensor Publishing Temperature (every 5 seconds):**
```json
Topic: devices/ESP32-IOT-SENSOR-AB12/telemetry
{
  "temperature": 23.4,
  "humidity": 58.7,
  "uptime": 456789,
  "wifiRSSI": -52,
  "mqttConnected": true,
  "freeHeap": 245000,
  "dataQuality": 98.5
}
```

**Dashboard Controlling GPIO:**
```json
Topic: device/ESP32-IOT-ACTUATOR-CD34/gpio/set
{
  "gpio": 3,
  "state": 1
}
```

**Automation Rule Trigger:**
```json
Topic: device/ESP32-IOT-ACTUATOR-CD34/gpio/set
{
  "gpio": 1,
  "state": 1
}
```

---

## 🤖 Automation & Gesture Control

### Creating Automation Rules

**Step-by-Step Guide:**

1. **Open Automation Tab** in dashboard
2. **Click "Create New Rule"** button
3. **Fill in rule details:**
   - **Rule Name**: Descriptive name (e.g., "AC Control")
   - **IF Section**:
     - Source Device: Select sensor (e.g., "Living Room Sensor")
     - Parameter: Choose metric (temperature, humidity, wifiRSSI)
     - Operator: Select comparison (>, <, >=, <=, ==)
     - Threshold: Enter numeric value (e.g., 28)
   - **THEN Section**:
     - Target Device: Select actuator (e.g., "Relay Controller")
     - GPIO Pin: Choose channel 1-8
     - State: ON or OFF
   - **Auto-Toggle**: Check if action should reverse when condition not met
4. **Click "Create Rule"**
5. **Enable rule** with toggle switch

**Example Rules:**

```yaml
Rule: Office Fan Control
IF: temperature > 28°C
THEN: GPIO 1 → ON (fan)
Auto-toggle: Yes

Rule: Greenhouse Irrigation
IF: humidity < 45%
THEN: GPIO 2 → ON (water pump)
Auto-toggle: Yes

Rule: WiFi Signal Monitor
IF: wifiRSSI < -75
THEN: GPIO 8 → ON (warning LED)
Auto-toggle: No
```

### Setting Up Gesture Control

**Step-by-Step Guide:**

1. **Open Gesture Control Tab**
2. **Allow camera access** when prompted by browser
3. **Click "Create New Gesture Rule"**
4. **Configure gesture mapping:**
   - **Gesture Type**: Select from 5 supported gestures
   - **Target Device**: Choose actuator device
   - **GPIO Pin**: Select channel 1-8
   - **State**: ON or OFF
5. **Enable rule** with toggle switch
6. **Test gesture** in front of camera

**Example Gesture Mappings:**

```yaml
Gesture: Open Palm
Device: Living Room Actuator
GPIO: 1 (Light)
State: ON

Gesture: Fist
Device: Living Room Actuator
GPIO: 1 (Light)
State: OFF

Gesture: Thumbs Up
Device: Bedroom Actuator
GPIO: 3 (Fan)
State: ON

Gesture: Peace Sign
Device: Kitchen Actuator
GPIO: 2 (Coffee Maker)
State: ON
```

**Gesture Recognition Parameters:**
- Confidence threshold: 85%
- Cooldown period: 2 seconds
- Detection rate: ~30 FPS
- Hand landmarks: 21 points tracked

---

## 🔧 Troubleshooting

### ESP32 Device Issues

#### Device Not Entering AP Mode (Blue LED)

**Problem**: Device doesn't broadcast WiFi network

**Solutions:**
1. Hold BOOT button for 3 seconds (factory reset)
2. Verify NeoPixel LED is working (check GPIO 45)
3. Re-flash firmware via USB
4. Check serial monitor for error messages

#### WiFi Connection Fails (Red LED)

**Problem**: Device can't connect to WiFi

**Solutions:**
1. Verify WiFi credentials (case-sensitive SSID)
2. Ensure 2.4 GHz network (ESP32 doesn't support 5 GHz)
3. Check WiFi password is correct
4. Move device closer to router
5. Check router MAC filtering/access control
6. Try different WiFi channel on router

#### MQTT Not Connecting (Yellow LED)

**Problem**: WiFi connected but MQTT fails

**Solutions:**
1. Verify MQTT server IP address is correct (not localhost)
2. Check server is running: `npm start` in Web-Server/server
3. Verify port 1883 is open (firewall settings)
4. Check server IP hasn't changed (DHCP)
5. Restart MQTT broker
6. Check serial monitor for MQTT error codes

#### Sensor Reading Errors

**Problem**: Invalid temperature/humidity values

**Solutions:**
1. Check DHT20 wiring (SDA: GPIO 11, SCL: GPIO 12)
2. Verify I²C pull-up resistors (usually built-in)
3. Run diagnostics API: `http://192.168.4.1/api/diagnostics`
4. Check I²C bus scanning results
5. Replace DHT20 sensor if faulty
6. Verify 3.3V power supply to sensor

### Web Server Issues

#### Server Won't Start

**Problem**: npm start fails

**Solutions:**
```bash
# Check Node.js version
node --version  # Should be 16.0.0+

# Clear npm cache
npm cache clean --force

# Reinstall dependencies
rm -rf node_modules package-lock.json
npm install

# Check port availability
netstat -ano | findstr :3000
netstat -ano | findstr :1883
```

#### Devices Not Appearing in Dashboard

**Problem**: Dashboard shows 0 devices

**Solutions:**
1. Check ESP32 devices have green LEDs (MQTT connected)
2. Verify all devices on same network
3. Open browser console (F12) for errors
4. Check MQTT WebSocket connection status
5. Refresh page (Ctrl+F5)
6. Restart server

#### Gesture Recognition Not Working

**Problem**: Camera not detecting gestures

**Solutions:**
1. Allow camera access in browser settings
2. Use Chrome browser (best MediaPipe support)
3. Check camera is not used by another app
4. Improve lighting conditions
5. Show only one hand at a time
6. Hold gesture for full 1 second
7. Check browser console for MediaPipe errors

### Network Issues

#### Can't Find Server IP Address

**Windows:**
```powershell
ipconfig
# Look for "IPv4 Address" under WiFi adapter
```

**macOS/Linux:**
```bash
ifconfig
# or
ip addr show
# Look for inet address (not 127.0.0.1)
```

#### Port Already in Use

**Problem**: Port 3000 or 1883 already occupied

**Solutions:**

**Windows:**
```powershell
# Find process using port
netstat -ano | findstr :3000
# Kill process
taskkill /PID <PID> /F
```

**macOS/Linux:**
```bash
# Find and kill process
lsof -ti:3000 | xargs kill -9
lsof -ti:1883 | xargs kill -9
```

Or modify ports in `server.js`:
```javascript
const HTTP_PORT = 3001;     // Change from 3000
const MQTT_TCP_PORT = 1884; // Change from 1883
```

### Common Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `MQTT connection failed (rc=-2)` | Wrong server IP | Verify MQTT server address |
| `WiFi connection timeout` | Wrong credentials | Check SSID/password |
| `DHT20 sensor not found` | I²C bus error | Check wiring, run diagnostics |
| `WebSocket connection failed` | Server not running | Start server with `npm start` |
| `Camera permission denied` | Browser blocked access | Allow camera in browser settings |

### Serial Monitor Debugging

To view detailed ESP32 logs:

1. Connect ESP32 via USB
2. In VS Code, click PlatformIO icon
3. Click "Monitor" under project tasks
4. Baud rate: 115200

**Useful commands to check:**
- WiFi connection status
- MQTT connection attempts
- Sensor reading values
- Memory usage
- Error messages

---

## 📄 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2025 ESP32 IoT Fleet Management System

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

- **Espressif Systems** - ESP32-S3 microcontroller platform
- **Arduino Community** - ESP32 Arduino Core framework
- **FreeRTOS** - Real-time operating system kernel
- **Aedes** - Lightweight MQTT broker for Node.js
- **Google MediaPipe** - Hand gesture recognition ML model
- **PlatformIO** - Professional embedded development platform

---

## 📚 Additional Resources

### Component-Specific Documentation

For detailed information about individual components:

- **ESP32 Sensor**: See [ESP32-Sensor/README.md](ESP32-Sensor/README.md)
- **ESP32 Actuator**: See [ESP32-Actuator/README.md](ESP32-Actuator/README.md)
- **Web Server**: See [Web-Server/README.md](Web-Server/README.md)

### External Documentation

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [MQTT Protocol Specification](https://mqtt.org/mqtt-specification/)
- [MediaPipe Hands Guide](https://google.github.io/mediapipe/solutions/hands.html)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)

---

## 📞 Support & Contact

- **GitHub Repository**: [FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS)
- **Issues**: Report bugs or request features via GitHub Issues
- **Discussions**: Share your projects and ask questions in GitHub Discussions

---

<div align="center">

**Made with ❤️ using ESP32-S3, Node.js, and FreeRTOS**

⭐ Star this repository if you found it helpful!

</div>
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
