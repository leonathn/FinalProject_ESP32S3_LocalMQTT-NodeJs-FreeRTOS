# ESP32 IoT Sensor System

**Production-ready ESP32-S3 IoT firmware with WiFi Manager, MQTT telemetry, DHT20 sensor, FreeRTOS multitasking, and web-based configuration**

[![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green.svg)](https://www.freertos.org/)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](LICENSE)

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [Project Structure](#-project-structure)
- [Hardware Requirements](#-hardware-requirements)
- [Wiring Guide](#-wiring-guide)
- [Software Setup](#-software-setup)
- [Quick Start](#-quick-start)
- [Configuration](#-configuration)
- [MQTT Topics](#-mqtt-topics)
- [API Endpoints](#-api-endpoints)
- [FreeRTOS Task Architecture](#-freertos-task-architecture)
- [Development Guide](#-development-guide)
- [Troubleshooting](#-troubleshooting)
- [Performance & Metrics](#-performance--metrics)
- [License & Credits](#-license--credits)


---

## 🎯 Overview

This is a **production-ready ESP32-S3 IoT firmware** designed for rapid deployment of sensor-based monitoring systems. Built with professional software engineering practices, it provides a complete solution for environmental monitoring with minimal configuration.

### What Makes This Special?

- **Zero-Touch Configuration**: Captive portal setup—no hardcoded credentials needed
- **Industrial-Grade Architecture**: FreeRTOS multitasking with proper synchronization primitives
- **Plug-and-Play**: Automatic MQTT broker discovery via mDNS
- **Self-Diagnosing**: Comprehensive system health checks and error reporting
- **Thread-Safe**: Mutex-protected I²C bus for reliable sensor communication
- **User-Friendly**: Modern web interface with real-time WiFi and MQTT scanning

### Technology Stack

| Layer | Technology |
|-------|-----------|
| **Hardware** | ESP32-S3 (dual-core Xtensa LX7, 240MHz) |
| **Framework** | Arduino Core for ESP32 |
| **RTOS** | FreeRTOS (native ESP-IDF) |
| **Communication** | WiFi (802.11 b/g/n), MQTT 3.1.1 |
| **Sensors** | DHT20 (I²C temperature & humidity) |
| **Indicators** | WS2812B NeoPixel RGB LED |
| **Storage** | NVS (Non-Volatile Storage) |
| **Web Server** | ESP32 WebServer (captive portal) |

---

## ✨ Key Features

### Core Capabilities
- ✅ **WiFi Manager** - AP mode captive portal for easy setup (no code changes needed)
- ✅ **MQTT Client** - Reliable telemetry publishing with QoS and last will testament
- ✅ **DHT20 Sensor** - Accurate temperature (±0.3°C) and humidity (±3% RH) readings via I²C
- ✅ **FreeRTOS Tasks** - Three concurrent tasks for sensors, UI, and MQTT communication
- ✅ **NeoPixel LED** - 5-color visual status indication (orange/red/blue/green/purple)
- ✅ **Web Configuration** - Modern, responsive single-page application
- ✅ **mDNS Support** - Device discoverable as `<device_id>.local` on network

### Advanced Features
- 🔒 **Thread-Safe I²C** - Mutex-protected sensor access prevents bus conflicts
- 📊 **Telemetry Queue** - Buffered data transmission with overflow protection
- 🔄 **Event Groups** - Synchronized connection state management across tasks
- ⚡ **Factory Reset** - Long-press BOOT button (3s) to clear configuration
- 🏥 **Diagnostics API** - Real-time system health monitoring and I²C bus scanning
- 📡 **WiFi Scanner** - Built-in network discovery with signal strength (RSSI)
- 🔍 **MQTT Discovery** - Automatic broker detection via mDNS (no manual IP entry)
- 📈 **Data Quality Metrics** - Quality score (0-100%) based on sensor validation
- 🔐 **Pairing Token** - Optional 6-digit code for device authentication

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
┌──────────────────────────────────────────────────────────┐
│                    ESP32-S3 DevKit                       │
│                                                          │
│  [3.3V] ────────────┬────────────────────── [DHT20 VCC]  │
│                     │                                    │
│  [GND]  ────────────┼───────────┬────────── [DHT20 GND]  │
│                     │           │                        │
│  [GPIO 11] (SDA) ───┼───────────┼────────── [DHT20 SDA]  │
│  [GPIO 12] (SCL) ───┼───────────┼────────── [DHT20 SCL]  │
│                     │           │                        │
│  [5V]   ────────────┘           └────────── [Pixel VCC]  │
│  [GND]  ─────────────────────────────────── [Pixel GND]  │
│  [GPIO 45] ─────────────────────────────── [Pixel DIN]   │
│                                                          │
└──────────────────────────────────────────────────────────┘
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

## 🌟 FreeRTOS Task Architecture

### Task Overview

This firmware uses **three concurrent FreeRTOS tasks** for optimal performance and responsiveness:

| Task | Core | Priority | Stack | Interval | Purpose |
|------|------|----------|-------|----------|---------|
| **TaskSensors** | 0 | 1 | 4096 | 1000ms | Read DHT20, validate data, queue telemetry |
| **TaskUI** | 0 | 1 | 2048 | 500ms | Update NeoPixel LED based on connection state |
| **TaskMQTT** | 1 | 2 | 8192 | 100ms | Process queue, publish to broker, handle reconnection |

### Task Details

#### 1. TaskSensors (Sensor Reading)
```cpp
// Responsibilities:
- Acquire I²C mutex
- Read DHT20 temperature and humidity
- Validate sensor data quality
- Calculate data quality score (0-100%)
- Queue telemetry data for MQTT task
- Handle sensor errors gracefully

// Thread-Safety:
- Uses xSemaphoreTake() before I²C access
- Prevents bus conflicts with other I²C operations
- Timeout: 100ms (returns error if mutex unavailable)
```

#### 2. TaskUI (User Interface)
```cpp
// Responsibilities:
- Monitor WiFi and MQTT connection state via event group
- Update NeoPixel LED color based on system status
- Provide visual feedback to user

// LED States:
🟠 Orange - AP mode (configuration needed)
🔴 Red    - Connecting to WiFi (STA mode active)
🔵 Blue   - WiFi connected, MQTT disconnected
🟢 Green  - Fully operational (WiFi + MQTT connected)
🟣 Purple - Error state (used during diagnostics)
```

#### 3. TaskMQTT (Network Communication)
```cpp
// Responsibilities:
- Dequeue telemetry data from sensor task
- Publish data to MQTT broker
- Handle MQTT reconnection automatically
- Process incoming MQTT commands (reboot, diagnostics)
- Maintain connection keep-alive

// Error Handling:
- Automatic reconnection with exponential backoff
- Queue overflow protection (drops oldest data)
- Connection state broadcast via event group
```

### Synchronization Primitives

```cpp
// 1. I²C Mutex
SemaphoreHandle_t i2cMutex;
Purpose: Protect I²C bus from concurrent access
Usage: Lock before DHT20 reads, diagnostics, or I²C scans

// 2. Telemetry Queue
QueueHandle_t telemetryQueue;
Purpose: Buffer sensor data for MQTT publishing
Capacity: 10 messages (overflow-safe)
Usage: Sensors enqueue, MQTT dequeues

// 3. Event Group
EventGroupHandle_t connectionEvents;
Bits: WIFI_CONNECTED_BIT | MQTT_CONNECTED_BIT
Purpose: Synchronize connection state across tasks
Usage: WiFi/MQTT handlers set bits, UI task reads
```

---

## 🚀 Development Guide

### Building from Source

```bash
# Clone repository
git clone <repository-url>
cd ESP32-Sensor

# Install dependencies (automatic via PlatformIO)
pio pkg install

# Build firmware
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

### Customization Guide

#### Changing Pin Assignments

Edit [include/config.h](include/config.h):
```cpp
#define SDA_PIN 11              // Change I²C data pin
#define SCL_PIN 12              // Change I²C clock pin
#define NEOPIXEL_WIFI 45        // Change LED pin
```

#### Adjusting Task Timing

Edit [include/config.h](include/config.h):
```cpp
#define SENSOR_READ_INTERVAL_MS 5000  // Sensor read frequency
#define UI_UPDATE_INTERVAL_MS 500     // LED update frequency
#define MQTT_LOOP_INTERVAL_MS 100     // MQTT loop frequency
```

#### Modifying MQTT Topics

Edit [src/mqtt_handler.cpp](src/mqtt_handler.cpp):
```cpp
String telemetryTopic = "devices/" + deviceId + "/telemetry";
String statusTopic = "devices/" + deviceId + "/status";
// Add your custom topics here
```

#### Adding New Sensors

1. **Add sensor library** to `platformio.ini`:
   ```ini
   lib_deps = 
       <existing libraries>
       author/NewSensorLib@^1.0.0
   ```

2. **Initialize sensor** in `setup()` ([src/main.cpp](src/main.cpp)):
   ```cpp
   newSensor.begin();
   ```

3. **Read sensor** in `TaskSensors` ([src/tasks.cpp](src/tasks.cpp)):
   ```cpp
   if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100))) {
       float value = newSensor.read();
       xSemaphoreGive(i2cMutex);
   }
   ```

### Code Quality Standards

- ✅ **Modular Design**: Each feature in separate `.cpp`/`.h` files
- ✅ **Comprehensive Comments**: Doxygen-style documentation
- ✅ **Error Handling**: All I²C/network operations have timeout/fallback
- ✅ **Memory Safety**: No dynamic allocation in tasks (stack-only)
- ✅ **Thread Safety**: All shared resources protected by mutexes
- ✅ **Logging**: Serial output for all major events (with severity levels)

---

## 📊 Performance & Metrics

### Memory Usage

```
Flash Usage:    ~850 KB / 8 MB   (10.6%)
SRAM Usage:     ~45 KB / 512 KB  (8.8%)
Free Heap:      ~230-250 KB (runtime)
```

### Task Performance

| Metric | Value |
|--------|-------|
| **Sensor Read Latency** | ~50-80ms (I²C transaction) |
| **MQTT Publish Latency** | ~5-15ms (local network) |
| **LED Update Rate** | 2 Hz (500ms interval) |
| **Telemetry Rate** | 1 Hz (1 sensor reading/sec) |
| **Queue Depth** | Typically 0-2 messages (max 10) |

### Power Consumption

| Mode | Current | Power (3.3V) |
|------|---------|--------------|
| **Active (WiFi + MQTT)** | ~120-180mA | ~0.4-0.6W |
| **Idle (WiFi connected)** | ~80-100mA | ~0.26-0.33W |
| **Deep Sleep** | ~10-50µA | ~0.033-0.165mW |

*Note: Deep sleep not implemented in current version*

### Network Performance

- **WiFi Connection Time**: 3-8 seconds (depends on AP)
- **MQTT Connection Time**: 0.5-2 seconds (local broker)
- **Reconnection Interval**: 5 seconds (automatic retry)
- **Data Throughput**: ~1 message/sec (typical telemetry)

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

## 🔒 Security Considerations

### Current Implementation

⚠️ **This firmware is designed for LOCAL NETWORKS ONLY**. It implements basic security suitable for home/lab environments:

| Feature | Status | Notes |
|---------|--------|-------|
| **WiFi Encryption** | ✅ WPA2 | Uses saved network credentials |
| **MQTT Encryption** | ❌ None | Plain TCP (port 1883) |
| **MQTT Authentication** | ⚠️ Optional | Requires code modification |
| **Web Interface Auth** | ❌ None | Open access to config portal |
| **OTA Updates** | ❌ Not implemented | Firmware updates via USB only |

### Recommendations for Production

If deploying in production or public networks, consider:

1. **Enable MQTT TLS/SSL**
   ```cpp
   // Use WiFiClientSecure instead of WiFiClient
   WiFiClientSecure espClient;
   espClient.setCACert(ca_cert);  // Add broker certificate
   ```

2. **Add MQTT Authentication**
   ```cpp
   mqttClient.connect(deviceId.c_str(), "username", "password");
   ```

3. **Secure Web Interface**
   - Add HTTP Basic Auth or session tokens
   - Use HTTPS (requires certificate management)
   - Disable web server after initial configuration

4. **Implement OTA Updates**
   - Use ArduinoOTA or ESP32 HTTPS OTA
   - Verify firmware signatures before flashing

5. **Harden WiFi Configuration**
   - Disable AP mode after initial setup
   - Use WPA3 if supported by network
   - Implement MAC address filtering

---

## 🎓 Best Practices & Tips

### Deployment Tips

1. **Test on Bench First**
   - Verify all hardware connections
   - Run diagnostics endpoint before deployment
   - Monitor serial output for 5-10 minutes

2. **Network Configuration**
   - Use static IP reservation in router (DHCP reservation)
   - Document device ID and IP address
   - Add device to monitoring dashboard

3. **MQTT Broker Setup**
   - Enable persistence for retained messages
   - Set max message size ≥ 512 bytes
   - Configure QoS 1 for telemetry reliability

4. **Maintenance**
   - Check free heap memory periodically (`/api/status`)
   - Monitor error count in sensor readings
   - Review MQTT broker logs for connection issues

### Common Pitfalls

❌ **Don't:**
- Use long MQTT publish intervals (>10s) - increases latency
- Run without pull-up resistors on long I²C cables (>30cm)
- Deploy without testing factory reset procedure
- Forget to document device ID and pairing token

✅ **Do:**
- Add external pull-up resistors (4.7kΩ) for I²C reliability
- Use shielded cables for NeoPixel in high-EMI environments
- Keep MQTT payloads under 256 bytes for efficiency
- Implement MQTT last will testament for monitoring

---

## 🗺️ Roadmap & Future Enhancements

### Planned Features (v3.0)

- [ ] **Deep Sleep Mode** - Battery operation with wake-on-timer
- [ ] **Multiple Sensors** - Support for BME280, SHT31, MCP9808
- [ ] **Data Logging** - Local SPIFFS/SD card storage
- [ ] **OTA Updates** - Wireless firmware updates
- [ ] **TLS/SSL** - Encrypted MQTT communication
- [ ] **Home Assistant** - MQTT Discovery support
- [ ] **Grafana Dashboard** - Pre-built monitoring template
- [ ] **Alert System** - Temperature/humidity threshold notifications

### Contribution Ideas

Want to contribute? Here are some areas that need work:

- 🎨 **Web UI improvements** - Add charts, historical data view
- 📊 **Data analytics** - Moving averages, trend detection
- 🔔 **Alert system** - Configurable thresholds and notifications
- 🌐 **Internationalization** - Multi-language support
- 📱 **Mobile app** - Native Android/iOS companion app
- 🧪 **Unit tests** - Automated testing framework

---

## 📝 License

This project is open-source and available for educational and commercial use under the MIT License.

---

## 🙏 Credits

### Author
**Final Project - Embedded Systems**  
ESP32-S3 IoT Sensor System  
© 2025 | Built with ❤️ for learning and innovation

### Libraries & Dependencies

This project wouldn't be possible without these excellent open-source libraries:

| Library | Version | Purpose | License |
|---------|---------|---------|---------|
| [PubSubClient](https://github.com/knolleary/pubsubclient) | 2.8+ | MQTT client protocol implementation | MIT |
| [ArduinoJson](https://arduinojson.org/) | 7.0.3+ | JSON serialization/deserialization | MIT |
| [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | 1.12.0+ | WS2812B RGB LED driver | LGPL |
| [DHT20](https://github.com/RobTillaart/DHT20) | 0.2.1+ | Temperature/humidity sensor I²C driver | MIT |

### Frameworks & Platforms

- **[PlatformIO](https://platformio.org/)** - Professional embedded development platform
- **[ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)** - Arduino framework for ESP32
- **[FreeRTOS](https://www.freertos.org/)** - Real-time operating system (integrated in ESP-IDF)
- **[Espressif ESP-IDF](https://github.com/espressif/esp-idf)** - Official development framework

### Hardware

- **[Espressif ESP32-S3](https://www.espressif.com/en/products/socs/esp32-s3)** - Powerful dual-core WiFi/BLE SoC
- **[DHT20 Sensor](https://www.aosong.com/en/products-40.html)** - Digital temperature & humidity sensor
- **[WS2812B NeoPixel](https://www.adafruit.com/product/1655)** - Addressable RGB LED

---

## 📞 Support & Contact

### Getting Help

- **📖 Documentation**: You're reading it! Check troubleshooting section above
- **🐛 Issues**: Found a bug? Open an issue on GitHub
- **💡 Feature Requests**: Have an idea? Submit a feature request
- **❓ Questions**: Check existing issues or start a discussion

### Useful Resources

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [DHT20 Datasheet](https://www.aosong.com/m/en/products-32.html)
- [MQTT Protocol Specification](https://mqtt.org/mqtt-specification/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)

---

## ⭐ Acknowledgments

Special thanks to:
- The Arduino and ESP32 community for extensive documentation
- PlatformIO team for an amazing development platform
- All open-source contributors who made this project possible
- Students and makers learning embedded systems

---

<div align="center">

**🌟 If this project helped you, consider giving it a star! 🌟**

**Device ID Format:** `ESP32-IOT-SENSORS-xxxx` (auto-generated from MAC)  
**Firmware Version:** `v2.0`  
**Last Updated:** December 2025

---

Made with 💻 and ☕ | [View on GitHub](https://github.com/leonathn/FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS)

</div>

