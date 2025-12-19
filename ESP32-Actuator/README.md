# ESP32 Actuator Firmware

This is the actuator version of the ESP32 firmware. It **subscribes** to MQTT topics and controls actuators based on received commands.

## ⚠️ Important: Separate Compilation

The actuator code is designed to be compiled separately from the sensor firmware. To use it:

### Option 1: Create Separate Project
1. Create a new folder: `FinalProject_ESP32_Actuator`
2. Copy all files from `src/actuator/` to the new project's `src/` folder
3. Copy all files from `src/actuator/include/` to the new project's `include/` folder  
4. Copy `platformio.ini` and modify if needed
5. Build and upload independently

### Option 2: Use Build Flags (Advanced)
Modify `platformio.ini` to create two environments:
```ini
[env:esp32-sensor]
build_flags = -D BUILD_SENSOR

[env:esp32-actuator]
build_flags = -D BUILD_ACTUATOR
build_src_filter = +<actuator/>
```

### Option 3: Manually Swap Files
1. Backup current `src/` folder
2. Replace with actuator version when needed
3. Restore sensor version when done

## Key Differences from Sensor Firmware

### Removed Components
- ❌ **Sensor Reading Task** - No DHT20 sensor reading
- ❌ **Telemetry Publishing** - Does not publish sensor data
- ❌ **Sensor Tab** - Web interface simplified
- ❌ **I2C Communication** - No I2C mutex or sensor hardware

### Added Components
- ✅ **Command Subscription** - Listens to MQTT commands
- ✅ **Actuator Control** - Can control relays, LEDs, motors, etc.
- ✅ **Command Processing** - Processes incoming MQTT messages
- ✅ **Command Queue** - FreeRTOS queue for command buffering

## MQTT Topics

### Subscribed Topics
- `devices/{deviceId}/cmd` - Receives commands (LED, relay, etc.)
- `devices/{deviceId}/config` - Receives configuration updates

### Example Commands

**LED Control:**
```json
{
  "type": "led",
  "state": true
}
```

**Relay Control:**
```json
{
  "type": "relay",
  "state": true
}
```

**NeoPixel Color:**
```json
{
  "type": "neopixel",
  "color": {"r": 255, "g": 0, "b": 0}
}
```

## Hardware Setup

### Default Pin Configuration
- **NeoPixel LED**: GPIO 45 (status indicator)
- **Relay**: GPIO 13 (example actuator)
- **LED**: GPIO 12 (example actuator)
- **Reset Button**: GPIO 0 (Boot button)

### Customization
Modify pin definitions in [include/config.h](include/config.h):
```cpp
#define RELAY_PIN 13
#define LED_PIN 12
```

## Code Structure

### Main Components

1. **main.cpp** - Setup and main loop, no sensor initialization
2. **tasks.cpp** - FreeRTOS tasks:
   - `TaskActuator` - Processes commands from queue
   - `TaskUI` - Updates NeoPixel status
   - `TaskMQTT` - Maintains MQTT connection
3. **globals.h** - Simplified globals (no sensor objects)
4. **config.h** - Actuator-specific pin definitions

### FreeRTOS Synchronization
- **commandMutex**: Protects command processing
- **commandQueue**: Buffers incoming MQTT commands (20 items)
- **connectionEvents**: Tracks WiFi and MQTT connectivity

## Usage

### 1. Build and Flash
```bash
cd FinalProject_ESP32_Actuator
platformio run --target upload
```

### 2. Configure WiFi and MQTT
Same as sensor version - use captive portal at `ACTUATOR-SETUP-<id>`

### 3. Send Commands
From Node-RED, dashboard, or MQTT client:
```javascript
// Publish to devices/actuator_731c/cmd
{
  "type": "relay",
  "state": true
}
```

### 4. Monitor Serial Output
```bash
platformio device monitor
```

## Example Use Cases

### Home Automation
- Control lights via MQTT
- Automated relay switching
- Visual status feedback

### Industrial Control
- Process automation
- Equipment on/off control
- Status monitoring

### IoT Projects
- Remote device control
- Automated responses to sensor data
- Integration with cloud services

## Troubleshooting

### Actuator Not Responding
- Check MQTT connection (NeoPixel color)
- Verify topic name matches device ID
- Check serial monitor for command reception

### Compilation Errors
- Ensure using actuator version files only
- Check all includes point to actuator/include/
- Verify no sensor-specific code remains

### MQTT Connection Failed
- Same as sensor version
- Check broker IP and port
- Verify WiFi connection

## Future Enhancements
- PWM support for motor speed control
- Servo motor control
- Multiple relay support
- Scheduled/timed actions
- Feedback/acknowledgment messages

