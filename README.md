# ESP32 IoT Sensor System

**ESP32-S3 IoT device with WiFi Manager, MQTT telemetry, DHT20 sensor, and web-based configuration**

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Project Structure](#-project-structure)
- [Hardware Requirements](#-hardware-requirements)
- [Wiring Guide](#-wiring-guide)
- [Software Setup](#-software-setup)
- [Quick Start](#-quick-start)
- [Configuration](#-configuration)
- [MQTT Topics](#-mqtt-topics)
- [Features](#-features)
- [API Endpoints](#-api-endpoints)
- [Troubleshooting](#-troubleshooting)

---

## 🎯 Overview

This is a production-ready ESP32-S3 IoT firmware featuring:

- **WiFi Manager** - AP mode for easy configuration via captive portal
- **MQTT Client** - Publishes sensor telemetry and receives commands
- **DHT20 Sensor** - Temperature and humidity monitoring via I²C
- **FreeRTOS Tasks** - Multi-threaded sensor reading, UI updates, and MQTT communication
- **NeoPixel Status LED** - Visual indication of device state
- **Web Configuration** - User-friendly web interface for setup
- **Diagnostics** - Built-in system health checks and I²C scanning

---

## 📂 Project Structure

```
ESP32-Sensor/
│
├── 📁 include/                     # Header files
│   ├── config.h                   # Pin definitions and constants
│   ├── types.h                    # Data structures and enums
│   ├── globals.h                  # Global variables and objects
│   ├── config_manager.h           # NVS storage functions
│   ├── wifi_manager.h             # WiFi AP/STA management
│   ├── mqtt_handler.h             # MQTT client functions
│   ├── neopixel_handler.h         # LED status indicator
│   ├── diagnostics.h              # System diagnostics
│   ├── web_server.h               # Web server and API
│   └── tasks.h                    # FreeRTOS task definitions
│
├── 📁 src/                         # Implementation files
│   ├── main.cpp                   # Main program entry point
│   ├── config_manager.cpp         # Configuration persistence
│   ├── wifi_manager.cpp           # WiFi connection handling
│   ├── mqtt_handler.cpp           # MQTT messaging
│   ├── neopixel_handler.cpp       # LED control
│   ├── diagnostics.cpp            # System health checks
│   ├── web_server.cpp             # Web UI and API handlers
│   └── tasks.cpp                  # FreeRTOS task implementations
│
├── 📁 boards/                      # Custom board definitions
│   └── yolo_uno.json              # Custom board configuration
│
├── 📄 platformio.ini               # PlatformIO project configuration
└── 📄 README.md                    # This file
```

---

## 🔧 Hardware Requirements

| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP32-S3 DevKit | 1 | Main microcontroller |
| DHT20 I²C Sensor | 1 | Temperature + Humidity sensor |
| WS2812B NeoPixel | 1 | RGB Status LED |
| Breadboard | 1 | For prototyping |
| Jumper Wires | 8 | Male-to-male or male-to-female |

### Optional Components
- 0.96" I²C OLED Display (future expansion)
- Additional I²C sensors (can share same bus)

---

## 🔌 Wiring Guide

### Pin Assignments

```cpp
#define SDA_PIN 11           // I²C Data Line
#define SCL_PIN 12           // I²C Clock Line
#define NEOPIXEL_WIFI 45     // WiFi Status LED
#define DHT20_ADDR 0x38      // DHT20 I²C Address
#define RESET_BUTTON_PIN 0   // Boot button (hold 3s to reset config)
```

### Wiring Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    ESP32-S3 DevKit                       │
│                                                          │
│  [3.3V] ────────────┬───────────────────── [DHT20 VCC] │
│                     │                                    │
│  [GND]  ────────────┼───────────┬───────── [DHT20 GND] │
│                     │           │                        │
│  [GPIO 11] (SDA) ───┼───────────┼───────── [DHT20 SDA] │
│  [GPIO 12] (SCL) ───┼───────────┼───────── [DHT20 SCL] │
│                     │           │                        │
│  [5V]   ────────────┘           └───────── [Pixel VCC] │
│  [GND]  ────────────────────────────────── [Pixel GND] │
│  [GPIO 45] ──────────────────────────────── [Pixel DIN] │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

### Detailed Connections

**DHT20 Temperature/Humidity Sensor:**
```
DHT20 Module → ESP32-S3
────────────────────────
VCC (Red)    → 3.3V
GND (Black)  → GND
SDA (Yellow) → GPIO 11
SCL (White)  → GPIO 12
```

**WS2812B NeoPixel LED:**
```
NeoPixel → ESP32-S3
──────────────────
VCC (Red)  → 5V (or 3.3V)
GND (Black)→ GND
DIN (Green)→ GPIO 45
```

**Notes:**
- DHT20 operates at I²C address `0x38` (factory default)
- Most DHT20 modules have built-in pull-up resistors
- If sensor not detected, add 4.7kΩ pull-ups on SDA/SCL to 3.3V
- NeoPixel can run on 5V for brightness or 3.3V (lower brightness)

---

## 💻 Software Setup

### Prerequisites

1. **PlatformIO IDE** (VS Code extension) or **PlatformIO Core**
2. **USB Driver** for ESP32-S3 (usually CH340 or CP2102)

### Installation Steps

1. **Clone or Download** this repository
2. **Open Project** in PlatformIO
3. **Install Dependencies** (automatic via platformio.ini)
4. **Configure Board** (if not using esp32-s3-devkitc-1):
   ```ini
   [env:your_board]
   platform = espressif32
   board = your_board_name
   ```
5. **Update COM Port** in platformio.ini:
   ```ini
   upload_port = COM7      # Change to your port
   monitor_port = COM7     # Change to your port
   ```

### Build and Upload

```bash
# Build firmware
platformio run

# Upload to ESP32
platformio run --target upload

# Open serial monitor
platformio device monitor --baud 115200
```

---

## ⚡ Quick Start

### Step 1: First Boot (AP Mode)

1. Upload firmware to ESP32-S3
2. ESP32 creates WiFi hotspot: `ESP32-IOT-SENSORS-xxxx`
3. **Password:** `12345678`
4. LED shows **orange** (AP mode active)

### Step 2: Connect to ESP32

1. On your phone/laptop, connect to `ESP32-IOT-SENSORS-xxxx`
2. Open browser and go to: **http://192.168.4.1**
3. You'll see the configuration web interface

### Step 3: Configure WiFi

1. Click **"WiFi" tab**
2. Click **"🔍 Scan WiFi Networks"**
3. Select your home/office WiFi network
4. Enter WiFi password

### Step 4: Configure MQTT Broker

1. Click **"MQTT Broker" tab**
2. Click **"🔍 Scan for MQTT Brokers"** (uses mDNS)
   - OR manually enter your server IP and port
3. **Find your computer's IP:**
   - Windows: Open PowerShell → `ipconfig` → Look for "IPv4 Address"
   - Linux/Mac: `ifconfig` or `ip addr`
4. Enter:
   - **MQTT Server:** Your computer's local IP (e.g., `192.168.1.100`)
   - **MQTT Port:** `1883` (default)
   - **Pairing Token:** 6-digit code (optional, default: `123456`)

### Step 5: Save and Reboot

1. Go back to **"WiFi" tab**
2. Click **"💾 Save Configuration & Reboot"**
3. ESP32 reboots and connects to your WiFi
4. LED changes:
   - 🔴 **Red** - Connecting to WiFi
   - 🔵 **Blue** - WiFi connected, connecting to MQTT
   - 🟢 **Green** - Everything working!

### Step 6: Verify Connection

1. Check serial monitor for connection status
2. Device should publish telemetry to MQTT broker
3. Topics: `devices/<device_id>/telemetry`, `devices/<device_id>/status`

---

## ⚙️ Configuration

### Reset Configuration

To reset device to factory defaults:

1. **Method 1 - Button:** Hold BOOT button (GPIO 0) for **3 seconds**
   - LED blinks red rapidly
   - Configuration cleared
   - Device reboots to AP mode

2. **Method 2 - Web API:** Send POST to `/api/reset`

### Configuration Storage

All settings are stored in **NVS (Non-Volatile Storage)**:
- WiFi SSID and password
- MQTT server IP/hostname and port
- Pairing token

### LED Status Indicators

| Color | Status |
|-------|--------|
| 🟠 Orange | AP mode (configuration needed) |
| 🔴 Red | Connecting to WiFi |
| 🔵 Blue | WiFi connected, MQTT disconnected |
| 🟢 Green | Fully operational (WiFi + MQTT) |

---

## 📡 MQTT Topics

### Published by ESP32

#### Status Topic (Retained)
**Topic:** `devices/<device_id>/status`

**Payload:**
```json
{
  "online": true,
  "ip": "192.168.1.101",
  "rssi": -45,
  "wifiMode": "STA",
  "ts": 123456
}
```

**Last Will (offline):**
```json
{
  "online": false,
  "ts": 123456
}
```

#### Telemetry Topic
**Topic:** `devices/<device_id>/telemetry`  
**Frequency:** Every 1 second

**Payload:**
```json
{
  "tC": 25.3,           // Temperature in Celsius
  "rh": 65.2,           // Relative humidity in %
  "heap": 234567,       // Free heap memory in bytes
  "uptime": 3600,       // Uptime in seconds
  "quality": 100,       // Data quality (0-100%)
  "valid": true,        // Overall data validity
  "ts": 123456          // Timestamp in milliseconds
}
```

#### Pairing Topic
**Topic:** `devices/<device_id>/pair`

**Payload:**
```json
{
  "token": "123456",
  "ts": 123456
}
```

#### Diagnostics Topic
**Topic:** `devices/<device_id>/diagnostics`

**Payload:**
```json
{
  "i2c": "0x38",
  "dht20": {
    "ok": true,
    "tC": 25.3,
    "rh": 65.2
  },
  "neopixel": {
    "ok": true
  },
  "ts": 123456
}
```

### Subscribed Topics

#### Command Topic
**Topic:** `devices/<device_id>/cmd`

**Reboot Command:**
```json
{"cmd": "reboot"}
```

**Run Diagnostics:**
```json
{"cmd": "diagnostics"}
```

#### Configuration Topic
**Topic:** `devices/<device_id>/config`

(Reserved for future GPIO configuration)

---

## 🌟 Features

### Core Features
- ✅ **WiFi Manager** - AP mode captive portal for easy setup
- ✅ **MQTT Client** - Reliable telemetry publishing with QoS
- ✅ **DHT20 Sensor** - Accurate temperature and humidity readings
- ✅ **FreeRTOS Tasks** - Concurrent sensor reading and communication
- ✅ **NeoPixel LED** - Visual status indication
- ✅ **Web Configuration** - Modern, responsive web interface
- ✅ **mDNS Support** - Device discoverable as `<device_id>.local`

### Advanced Features
- ✅ **Mutex-Protected I²C** - Thread-safe sensor access
- ✅ **Telemetry Queue** - Buffered data with overflow protection
- ✅ **Event Groups** - Synchronized connection state management
- ✅ **Configuration Reset** - Long-press button to factory reset
- ✅ **Diagnostics API** - System health monitoring
- ✅ **WiFi Scanner** - Built-in network discovery
- ✅ **MQTT Broker Discovery** - Automatic mDNS scanning

### FreeRTOS Architecture

**Three concurrent tasks:**

1. **TaskSensors** (Core 0, Priority 1)
   - Reads DHT20 sensor every 1 second
   - Validates data quality
   - Queues telemetry for transmission

2. **TaskUI** (Core 0, Priority 1)
   - Updates NeoPixel LED every 500ms
   - Reflects system connection state

3. **TaskMQTT** (Core 1, Priority 2)
   - Processes telemetry queue
   - Publishes to MQTT broker
   - Handles MQTT reconnection

---

## 🔧 API Endpoints

### Web Server Endpoints

**Base URL:** `http://192.168.4.1` (AP mode) or `http://<device_ip>` (STA mode)

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Web configuration interface |
| `/save` | POST | Save WiFi and MQTT configuration |
| `/api/status` | GET | Get device status |
| `/api/diagnostics/run` | GET | Run system diagnostics |
| `/api/wifi/scan` | GET | Scan for WiFi networks |
| `/api/mqtt/scan` | GET | Scan for MQTT brokers (mDNS) |
| `/api/sensors` | GET | Get current sensor readings |
| `/api/reset` | POST | Reset configuration and reboot |

### Example API Responses

**GET /api/status**
```json
{
  "mode": "STA",
  "wifi": true,
  "mqtt": true,
  "uptime": 3600,
  "heap": 234567,
  "ip": "192.168.1.101",
  "rssi": -45
}
```

**GET /api/sensors**
```json
{
  "dht20Ok": true,
  "temperature": 25.3,
  "humidity": 65.2,
  "uptime": 3600,
  "heap": 234567,
  "readCount": 3600,
  "errorCount": 5
}
```

---

## 🐛 Troubleshooting

### Device Won't Connect to WiFi

1. **Check network name:** Ensure SSID is correct (case-sensitive)
2. **Check password:** Verify WiFi password
3. **Check signal strength:** Move ESP32 closer to router
4. **Serial monitor:** Check for error messages
5. **Reset config:** Hold BOOT button for 3 seconds

### MQTT Not Connecting

1. **Verify broker IP:** Use computer's local IP, not `localhost`
2. **Check firewall:** Allow port 1883 on your computer
3. **Test broker:** Use MQTT client to verify broker is running
4. **Check credentials:** If broker requires auth, update code

### Sensor Not Reading

1. **Check wiring:** Verify I²C connections (SDA=11, SCL=12)
2. **Check power:** Ensure 3.3V is supplied to DHT20
3. **I²C scan:** Check serial monitor for detected devices
4. **Pull-ups:** Add 4.7kΩ resistors on SDA/SCL if needed

### LED Not Working

1. **Check wiring:** Verify DIN connected to GPIO 45
2. **Power:** Ensure NeoPixel has 5V or 3.3V power
3. **Order:** Check data flow direction (DIN → DOUT)

### Can't Access Web Interface

1. **AP Mode:** Ensure connected to ESP32 WiFi
2. **Captive Portal:** Should auto-open, or manually go to `192.168.4.1`
3. **STA Mode:** Use device's IP address from serial monitor

### Serial Monitor Shows Errors

| Error | Solution |
|-------|----------|
| `I2C timeout` | Check sensor wiring and pull-ups |
| `WiFi failed` | Verify SSID/password, check signal |
| `MQTT failed` | Check broker IP and port |
| `Mutex timeout` | Normal occasional message, ignore if rare |

---

## 📝 License

This project is open-source and available for educational and commercial use.

---

## 🙏 Credits

**Libraries Used:**
- [PubSubClient](https://github.com/knolleary/pubsubclient) - MQTT client
- [ArduinoJson](https://arduinojson.org/) - JSON parsing
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - LED control
- [DHT20](https://github.com/RobTillaart/DHT20) - Temperature/humidity sensor

**Built with:** PlatformIO, ESP32 Arduino Core, FreeRTOS

---

**Device ID:** Auto-generated from MAC address (`ESP32-IOT-SENSORS-xxxx`)  
**Firmware Version:** 2.0  
**Last Updated:** December 2025
- ✅ Real-time device monitoring
- ✅ Telemetry visualization (Recharts)

### ESP32 Firmware
- ✅ WiFi Manager with AP mode captive portal
- ✅ FreeRTOS task architecture (Sensors, UI, MQTT)
- ✅ Semaphore-protected I2C bus
- ✅ DHT20 temperature/humidity sensor
- ✅ NeoPixel status LED with color codes
- ✅ Comprehensive diagnostics
- ✅ NVS persistent configuration

---

## 🔌 Hardware Requirements

- **ESP32-S3** (or compatible)
- **DHT20** I2C sensor (optional)
- **WS2812B NeoPixel** LED (optional)

See [WIRING_GUIDE.md](WIRING_GUIDE.md) for pin connections.

---

## 📖 Documentation

| Document | Purpose |
|----------|---------|
| [INDEX.md](INDEX.md) | Navigation and use case guides |
| [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) | Complete overview and quick start |
| [README-FULL.md](README-FULL.md) | In-depth documentation and API reference |
| [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md) | Step-by-step verification |
| [WIRING_GUIDE.md](WIRING_GUIDE.md) | Hardware setup and troubleshooting |

---

## 🚀 System Architecture

```
┌─────────────┐      MQTT (TCP 1883)        ┌─────────────┐
│   ESP32-S3  │ ◄──────────────────────────► │  Node.js    │
│   Firmware  │                              │   Server    │
└─────────────┘                              └──────┬──────┘
                                                    │
                 MQTT WebSocket (9001)              │ HTTP REST
                                                    │ (3000)
                                             ┌──────▼──────┐
                                             │    React    │
                                             │  Dashboard  │
                                             └─────────────┘
```

---

## 📡 MQTT Topics

```
devices/{deviceId}/status      - Online/offline status (LWT)
devices/{deviceId}/telemetry   - Sensor data (temp, humidity, heap)
devices/{deviceId}/config      - Configuration updates
devices/{deviceId}/cmd         - Commands (reboot, diagnostics)
devices/{deviceId}/ack         - Command acknowledgments
devices/{deviceId}/pair        - Pairing token
devices/{deviceId}/diagnostics - Diagnostic results
```

---

## 🛠️ Development

### Server Development
```bash
cd server-v2
npm run dev  # Auto-restart on changes
```

### Web Development
```bash
cd web
npm run dev  # Vite HMR enabled
```

### Firmware Development
```bash
pio run              # Build
pio run -t upload    # Upload
pio device monitor   # Serial monitor
```

---

## 🐛 Troubleshooting

See [README-FULL.md#troubleshooting](README-FULL.md#-troubleshooting) for:
- ESP32 won't connect to WiFi
- Dashboard shows no devices
- MQTT connection issues
- Hardware problems

---

## 📊 Project Stats

- **Total Lines of Code:** ~2,500+
- **Languages:** C++, TypeScript, JavaScript
- **Firmware:** 725 lines (v2.0)
- **Server:** 201 lines
- **Documentation:** 7 comprehensive guides

---

## 📝 License

MIT License - Free to use in your projects!

---

## 🎉 Get Started Now!

1. **Read:** [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)
2. **Setup:** Follow quick start above
3. **Verify:** Use [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md)

**Total setup time:** ~1 hour to fully operational system

---

**Questions?** Check the documentation or see [INDEX.md](INDEX.md) for navigation.

**Happy Building! 🚀**
