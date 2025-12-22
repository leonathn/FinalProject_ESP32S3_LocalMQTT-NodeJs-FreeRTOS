# ESP32 Actuator - IoT MQTT Control System 🎛️

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32--S3-orange.svg)](https://platformio.org/)
[![ESP32-S3](https://img.shields.io/badge/ESP32--S3-DevKitC--1-blue.svg)](https://www.espressif.com/)
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Enabled-green.svg)](https://www.freertos.org/)
[![MQTT](https://img.shields.io/badge/MQTT-v3.1.1-purple.svg)](https://mqtt.org/)

A professional-grade IoT actuator firmware for ESP32-S3, designed to receive MQTT commands and control up to **8 GPIO channels** remotely. Perfect for home automation, industrial control systems, and IoT applications.

## 🌟 Features

### Core Functionality
- ✅ **8-Channel GPIO Control** - Individual control of 8 digital output pins
- ✅ **MQTT Command Subscription** - Real-time command processing via MQTT
- ✅ **Web Configuration Interface** - User-friendly setup portal with WiFi scanner
- ✅ **MQTT Broker Auto-Discovery** - Automatic detection of local MQTT brokers
- ✅ **FreeRTOS Multi-tasking** - Efficient task management for responsive operation
- ✅ **Visual Status Indicators** - NeoPixel LED showing connection status
- ✅ **Persistent Configuration** - Settings stored in non-volatile memory
- ✅ **Secure Pairing Token** - Optional 6-digit security code
- ✅ **Telemetry Publishing** - Real-time status updates to MQTT broker
- ✅ **Over-The-Air Updates** - Remote firmware updates via web interface

### Operational Modes
- **Access Point (AP) Mode** - For initial configuration without WiFi
- **Station (STA) Mode** - Normal operation connected to WiFi network
- **Factory Reset** - Hold boot button for 3 seconds to clear all settings

## 📋 Table of Contents

- [Hardware Requirements](#-hardware-requirements)
- [GPIO Pin Mapping](#-gpio-pin-mapping)
- [Software Architecture](#-software-architecture)
- [Installation](#-installation)
- [Configuration](#-configuration)
- [MQTT Topics & Commands](#-mqtt-topics--commands)
- [Web Interface](#-web-interface)
- [API Reference](#-api-reference)
- [Troubleshooting](#-troubleshooting)

## 🔌 Hardware Requirements

### Required Components
- **ESP32-S3 DevKitC-1** (or compatible board)
- **USB-C Cable** for programming and power
- **5V Power Supply** (for controlling external devices)

### Optional Components
- **8x Relay Module** (for switching high-power devices)
- **8x MOSFETs** (for PWM control or LED strips)
- **NeoPixel LED** (WS2812B) - Built-in on GPIO 45 on DevKitC-1
- **External LEDs/Relays** - Connected to control pins

### Specifications
- **Operating Voltage**: 3.3V (GPIO) / 5V (VIN)
- **Maximum Current per GPIO**: 40mA
- **Total GPIO Current**: 1200mA max
- **WiFi**: 802.11 b/g/n (2.4 GHz)
- **Flash Memory**: 8MB (minimum)

## 📌 GPIO Pin Mapping

### Output Channels (Configurable)
| Channel | Physical GPIO | Function | Default State |
|---------|---------------|----------|---------------|
| GPIO 1  | 5             | Digital Output | LOW |
| GPIO 2  | 6             | Digital Output | LOW |
| GPIO 3  | 7             | Digital Output | LOW |
| GPIO 4  | 8             | Digital Output | LOW |
| GPIO 5  | 9             | Digital Output | LOW |
| GPIO 6  | 10            | Digital Output | LOW |
| GPIO 7  | 21            | Digital Output | LOW |
| GPIO 8  | 38            | Digital Output | LOW |

### System Pins
| Pin | Function | Description |
|-----|----------|-------------|
| GPIO 45 | NeoPixel Status LED | Visual WiFi/MQTT status indicator |
| GPIO 0 | Boot/Reset Button | Hold 3s for factory reset |

### NeoPixel Status Colors
| Color | Status | Meaning |
|-------|--------|---------|
| 🔴 Red | Error | No WiFi or MQTT connection |
| 🟡 Yellow | WiFi Only | Connected to WiFi, no MQTT |
| 🟢 Green | Fully Connected | WiFi + MQTT operational |
| 🔵 Blue | AP Mode | Configuration mode active |

## 🏗️ Software Architecture

### FreeRTOS Tasks

```
┌─────────────────┐
│   TaskActuator  │ ← Command Queue ← MQTT Callback
│   Priority: 2   │   Processes GPIO commands
└─────────────────┘

┌─────────────────┐
│    TaskMQTT     │ ← Maintains MQTT connection
│   Priority: 2   │   Publishes telemetry
└─────────────────┘

┌─────────────────┐
│     TaskUI      │ ← Updates NeoPixel LED
│   Priority: 1   │   Visual status feedback
└─────────────────┘
```

### Core Modules

#### 1. **MQTT Handler** ([mqtt_handler.cpp](src/mqtt_handler.cpp))
- Manages MQTT broker connection
- Subscribes to command topics
- Publishes device status and telemetry
- Handles incoming JSON commands

#### 2. **Actuator Task** ([tasks.cpp](src/tasks.cpp))
- Processes GPIO control commands
- Manages command queue (FreeRTOS)
- Updates GPIO states
- Handles NeoPixel color changes

#### 3. **Web Server** ([web_server.cpp](src/web_server.cpp))
- Configuration portal interface
- WiFi network scanner
- MQTT broker auto-discovery
- REST API endpoints

#### 4. **WiFi Manager** ([wifi_manager.cpp](src/wifi_manager.cpp))
- WiFi connection management
- Access Point creation
- Captive portal for setup
- Connection monitoring

#### 5. **Configuration Manager** ([config_manager.cpp](src/config_manager.cpp))
- Non-volatile storage (NVS)
- WiFi credentials
- MQTT settings
- Device ID generation

## 🚀 Installation

### Prerequisites
- [PlatformIO](https://platformio.org/) installed (VS Code extension recommended)
- USB drivers for ESP32-S3
- Git (optional)

### Step 1: Clone or Download
```bash
git clone <repository-url>
```

### Step 2: Configure Upload Port
Edit [platformio.ini](platformio.ini) and set your COM port:
```ini
upload_port = COM9    ; Change to your port (check Device Manager)
monitor_port = COM9
```

### Step 3: Build and Upload
```bash
# Using PlatformIO CLI
pio run -t upload

# Or use VS Code: Click "Upload" button in PlatformIO toolbar
```

### Step 4: Monitor Serial Output
```bash
pio device monitor
# Or press Ctrl+Shift+P → "PlatformIO: Monitor"
```

## ⚙️ Configuration

### Initial Setup (First Boot)

1. **Power on the ESP32** - Device enters AP mode automatically
2. **Connect to WiFi AP**: `ESP32-IOT-ACTUATOR-XXXXXX` (password: `12345678`)
3. **Open browser**: Navigate to `http://192.168.4.1`
4. **Configure WiFi**:
   - Click "Scan WiFi Networks"
   - Select your network
   - Enter password
5. **Configure MQTT Broker**:
   - Option A: Click "Scan for MQTT Brokers" (auto-detects local brokers)
   - Option B: Enter IP manually (e.g., `192.168.1.100`)
   - Set port (default: `1883`)
6. **Optional Pairing Token**: Enter 6-digit code (or use default `123456`)
7. **Save & Reboot**: Device will restart and connect to WiFi

### Factory Reset

Hold the **Boot button (GPIO 0)** for **3 seconds** until NeoPixel flashes red twice. All settings will be erased.

## 📡 MQTT Topics & Commands

### Published Topics

#### Status Topic
```
devices/{deviceId}/status
```
Payload:
```json
{
  "online": true,
  "ip": "192.168.1.150",
  "rssi": -45,
  "wifiMode": "STA",
  "ts": 123456789
}
```

#### Telemetry Topic (Published every 10 seconds)
```
devices/{deviceId}/telemetry
```
Payload:
```json
{
  "gpio": [
    {"pin": 1, "state": false, "physicalPin": 5},
    {"pin": 2, "state": true, "physicalPin": 6},
    ...
  ],
  "uptime": 3600,
  "heap": 245000,
  "rssi": -47,
  "ts": 123456789
}
```

### Subscribed Topics

#### GPIO Control Topic
```
device/{deviceId}/gpio/set
```

**GPIO ON/OFF Command:**
```json
{
  "type": "gpio",
  "pin": 1,
  "state": true
}
```
- `pin`: 1-8 (channel number)
- `state`: `true` (HIGH) or `false` (LOW)

**Alternative (Relay):**
```json
{
  "type": "relay",
  "pin": 3,
  "state": false
}
```

#### NeoPixel Control
```json
{
  "type": "neopixel",
  "color": {
    "r": 255,
    "g": 128,
    "b": 0
  }
}
```

#### System Commands Topic
```
device/{deviceId}/cmd
```

**Reboot Command:**
```json
{
  "cmd": "reboot"
}
```

## 🌐 Web Interface

### Main Tabs

#### 1️⃣ WiFi Tab
- **WiFi Network Scanner** - Displays available networks with signal strength
- **Manual SSID Entry** - For hidden networks
- **Password Input** - Secure password field
- **Connection Status** - Real-time WiFi status

#### 2️⃣ MQTT Broker Tab
- **MQTT Broker Scanner** - Auto-discovers brokers on local network
- **Manual Configuration** - IP address and port input
- **Pairing Token** - Optional security code
- **Current Settings Display** - Shows active MQTT configuration

#### 3️⃣ GPIO Control Tab
- **Real-time GPIO States** - View all 8 channel states
- **Manual Toggle Controls** - Direct GPIO control from web UI
- **Physical Pin Mapping** - Shows GPIO to physical pin mapping
- **Live Status Updates** - Auto-refreshing GPIO states

### REST API Endpoints

#### GET `/api/status`
Returns device status:
```json
{
  "deviceId": "ESP32-XXXXXX",
  "wifi": {"connected": true, "ssid": "MyNetwork", "rssi": -45},
  "mqtt": {"connected": true, "server": "192.168.1.100"},
  "gpio": [false, true, false, true, false, false, false, false],
  "uptime": 3600,
  "heap": 245000
}
```

#### GET `/api/wifi/scan`
Scan WiFi networks:
```json
{
  "networks": [
    {"ssid": "MyWiFi", "rssi": -45, "encryption": "WPA2"},
    ...
  ]
}
```

#### GET `/api/mqtt/scan`
Scan for MQTT brokers:
```json
{
  "brokers": [
    {"ip": "192.168.1.100", "port": 1883, "type": "mosquitto"},
    ...
  ]
}
```

#### POST `/api/gpio`
Control GPIO:
```json
{
  "pin": 1,
  "state": true
}
```

#### POST `/api/reset`
Factory reset device.

## 🧪 Testing

### Test GPIO Control via MQTT

Using **mosquitto_pub** (or any MQTT client):

```bash
# Turn ON GPIO channel 1
mosquitto_pub -h 192.168.1.100 -t "device/ESP32-XXXXXX/gpio/set" -m '{"type":"gpio","pin":1,"state":true}'

# Turn OFF GPIO channel 1
mosquitto_pub -h 192.168.1.100 -t "device/ESP32-XXXXXX/gpio/set" -m '{"type":"gpio","pin":1,"state":false}'

# Control multiple channels
mosquitto_pub -h 192.168.1.100 -t "device/ESP32-XXXXXX/gpio/set" -m '{"type":"gpio","pin":5,"state":true}'

# Change NeoPixel color to red
mosquitto_pub -h 192.168.1.100 -t "device/ESP32-XXXXXX/gpio/set" -m '{"type":"neopixel","color":{"r":255,"g":0,"b":0}}'
```

### Using Node-RED

1. Add **MQTT Out** node
2. Configure broker: `192.168.1.100:1883`
3. Set topic: `device/ESP32-XXXXXX/gpio/set`
4. Inject message:
```json
{"type":"gpio","pin":1,"state":true}
```

## 📁 Project Structure

```
ESP32-Actuator/
├── platformio.ini          # PlatformIO configuration
├── README.md               # This file
├── include/
│   ├── config.h           # Pin definitions and constants
│   ├── config_manager.h   # NVS configuration management
│   ├── globals.h          # Global variables and objects
│   ├── mqtt_handler.h     # MQTT client interface
│   ├── neopixel_handler.h # NeoPixel status LED
│   ├── tasks.h            # FreeRTOS task definitions
│   ├── types.h            # Data structures
│   ├── web_server.h       # Web interface
│   └── wifi_manager.h     # WiFi management
├── src/
│   ├── main.cpp           # Main entry point
│   ├── config_manager.cpp # Configuration persistence
│   ├── mqtt_handler.cpp   # MQTT implementation
│   ├── neopixel_handler.cpp # LED status handler
│   ├── tasks.cpp          # FreeRTOS task logic
│   ├── web_server.cpp     # Web UI & API
│   └── wifi_manager.cpp   # WiFi connection logic
└── lib/                   # External libraries (PlatformIO managed)
```

## 🔧 Customization

### Change GPIO Pin Mapping

Edit [src/main.cpp](src/main.cpp):
```cpp
const uint8_t gpioOutputPins[8] = {
  5, 6, 7, 8,      // Channels 1-4
  9, 10, 21, 38    // Channels 5-8
};
```

### Modify Web Interface

Edit [src/web_server.cpp](src/web_server.cpp):
- HTML/CSS in `handleRoot()` function
- API endpoints in `setupWebServer()`

### Adjust Task Priorities

Edit [include/config.h](include/config.h):
```cpp
#define UI_UPDATE_INTERVAL_MS 500
#define MQTT_LOOP_INTERVAL_MS 100
```

### Change Access Point Credentials

Edit [include/config.h](include/config.h):
```cpp
#define AP_SSID_PREFIX "ESP32-IOT-ACTUATOR-"
#define AP_PASSWORD "12345678"
```

## 🐛 Troubleshooting

### Device Won't Connect to WiFi

**Symptoms**: NeoPixel stays red

**Solutions**:
1. Verify WiFi credentials in configuration
2. Check WiFi signal strength (RSSI > -70 dBm)
3. Ensure 2.4 GHz network (ESP32 doesn't support 5 GHz)
4. Factory reset and reconfigure

**Check Serial Monitor**:
```
[WiFi] Connecting to: MyNetwork
[WiFi] Failed! Retrying...
```

### MQTT Not Connecting

**Symptoms**: NeoPixel yellow, no commands received

**Solutions**:
1. Verify MQTT broker is running:
   ```bash
   netstat -an | findstr :1883
   ```
2. Check broker IP address is correct
3. Ensure broker allows anonymous connections
4. Verify firewall isn't blocking port 1883

**Serial Monitor**:
```
[MQTT] Connecting to: 192.168.1.100:1883
[MQTT] Failed, rc=-2
```

**Common Error Codes**:
- `-2`: Network unreachable
- `-4`: Connection timeout
- `5`: Connection refused

### GPIO Not Responding

**Symptoms**: Commands received but no output change

**Solutions**:
1. Check physical pin connections
2. Verify correct channel number (1-8)
3. Measure voltage at GPIO pin (should be 3.3V when HIGH)
4. Check GPIO pin isn't damaged
5. Review serial output for command processing:
   ```
   [Actuator] Received command: {"type":"gpio","pin":1,"state":true}
   [GPIO1] ✓ Pin 5 -> ON
   ```

### Web Interface Not Accessible

**Symptoms**: Can't reach 192.168.4.1 in AP mode

**Solutions**:
1. Ensure connected to ESP32 AP network
2. Disable mobile data on smartphone
3. Try `192.168.4.1` instead of hostname
4. Clear browser cache
5. Try different browser

### Upload Fails

**Symptoms**: `Timed out waiting for packet header`

**Solutions**:
1. Hold BOOT button during upload
2. Check USB cable (use data cable, not charge-only)
3. Install/update CH340 or CP2102 drivers
4. Try different USB port
5. Reduce upload speed in platformio.ini:
   ```ini
   upload_speed = 115200
   ```

### Device Keeps Rebooting

**Check Serial Monitor**:
```
rst:0x8 (TG1WDT_SYS_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
```

**Solutions**:
1. Power supply insufficient → Use 5V 2A adapter
2. Stack overflow → Increase stack size in task creation
3. Memory leak → Check heap with `ESP.getFreeHeap()`
4. Infinite loop in task → Review task code logic

## 📚 Dependencies

All dependencies are automatically managed by PlatformIO:

| Library | Version | Purpose |
|---------|---------|---------|
| PubSubClient | ^2.8 | MQTT client |
| ArduinoJson | ^7.0.3 | JSON parsing/serialization |
| Adafruit NeoPixel | ^1.12.0 | WS2812B LED control |

## 🔒 Security Considerations

### Best Practices
- ✅ Change default AP password in [config.h](include/config.h)
- ✅ Use unique pairing tokens for each device
- ✅ Enable MQTT authentication on broker
- ✅ Use TLS/SSL for production (requires additional configuration)
- ✅ Isolate IoT devices on separate VLAN

### Not Implemented (Add if needed)
- ⚠️ MQTT username/password authentication
- ⚠️ TLS/SSL encryption
- ⚠️ OTA password protection
- ⚠️ Web interface authentication

## 🚦 Status LED Reference

| Blink Pattern | Meaning |
|---------------|---------|
| Solid Blue | AP mode active (configuration) |
| Blinking Red (fast) | Connecting to WiFi |
| Solid Yellow | WiFi connected, MQTT disconnected |
| Solid Green | Fully operational (WiFi + MQTT) |
| Blinking Red (slow) | Error state |
| Red flash x2 | Factory reset triggered |

## 📖 Additional Resources

### MQTT Broker Setup
- **Mosquitto** (Windows): [Download](https://mosquitto.org/download/)
- **Docker**: `docker run -p 1883:1883 eclipse-mosquitto`
- **Node.js** (Aedes): See companion server project

### MQTT Clients
- **MQTT Explorer**: [Download](http://mqtt-explorer.com/)
- **MQTTX**: [Download](https://mqttx.app/)
- **mosquitto_pub/sub**: Command-line tools

### ESP32 Documentation
- [ESP32-S3 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)
- [PlatformIO ESP32](https://docs.platformio.org/en/latest/platforms/espressif32.html)

## 🤝 Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open Pull Request

## 📄 License

This project is part of a Final Project for Embedded Systems course.

## 👤 Author

**Hoai Nhan**
- GitHub: [@leonathn](https://github.com/leonathn)
- Project: FinalProject_ESP32S3_LocalMQTT-NodeJs-FreeRTOS

## 🙏 Acknowledgments

- Espressif Systems for ESP32-S3 platform
- PlatformIO team for excellent build system
- Arduino community for libraries
- MQTT protocol developers

---

**Made with ❤️ for IoT enthusiasts**

