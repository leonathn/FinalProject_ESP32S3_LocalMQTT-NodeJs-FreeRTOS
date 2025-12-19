# ESP32-IOT-ACTUATOR MQTT Topics

## Device Naming
- **Device ID Format:** `ESP32-IOT-ACTUATOR-xxxx` (where xxxx = last 2 bytes of MAC address)
- **Example:** `ESP32-IOT-ACTUATOR-4e5c`
- **Access Point SSID:** `ESP32-IOT-ACTUATOR-4e5c`

## MQTT Topics

### 📤 Publishing (Device → Server)

#### Status Topic
```
devices/ESP32-IOT-ACTUATOR-xxxx/status
```
Published when device connects to MQTT broker (retained message).

**Payload Example:**
```json
{
  "online": true,
  "ip": "192.168.1.100",
  "rssi": -45,
  "wifiMode": "STA",
  "ts": 123456
}
```

#### Telemetry Topic
```
devices/ESP32-IOT-ACTUATOR-xxxx/telemetry
```
Published every 10 seconds with GPIO states and system info.

**Payload Example:**
```json
{
  "gpio": [
    {"pin": 1, "state": false, "physicalPin": 5},
    {"pin": 2, "state": true, "physicalPin": 6},
    {"pin": 3, "state": false, "physicalPin": 7},
    {"pin": 4, "state": false, "physicalPin": 8},
    {"pin": 5, "state": false, "physicalPin": 9},
    {"pin": 6, "state": false, "physicalPin": 10},
    {"pin": 7, "state": false, "physicalPin": 21},
    {"pin": 8, "state": false, "physicalPin": 38}
  ],
  "uptime": 3600,
  "heap": 250000,
  "rssi": -45,
  "ts": 123456
}
```

### 📥 Subscribing (Server → Device)

#### GPIO Control Topic
```
device/ESP32-IOT-ACTUATOR-xxxx/gpio/set
```
Send GPIO control commands to this topic.

**Payload Example:**
```json
{
  "type": "gpio",
  "pin": 1,
  "state": true
}
```

**Parameters:**
- `type`: Must be "gpio"
- `pin`: GPIO channel number (1-8)
- `state`: true (ON) or false (OFF)

**GPIO Pin Mapping:**
| GPIO Channel | Physical Pin | ESP32 Pin |
|--------------|--------------|-----------|
| 1            | 5            | GPIO5     |
| 2            | 6            | GPIO6     |
| 3            | 7            | GPIO7     |
| 4            | 8            | GPIO8     |
| 5            | 9            | GPIO9     |
| 6            | 10           | GPIO10    |
| 7            | 21           | GPIO21    |
| 8            | 38           | GPIO38    |

#### General Commands Topic
```
device/ESP32-IOT-ACTUATOR-xxxx/cmd
```
Send general device commands.

**Payload Example (Reboot):**
```json
{
  "cmd": "reboot"
}
```

## Example MQTT Commands

### Turn GPIO 1 ON
```bash
mosquitto_pub -h localhost -t "device/ESP32-IOT-ACTUATOR-4e5c/gpio/set" -m '{"type":"gpio","pin":1,"state":true}'
```

### Turn GPIO 3 OFF
```bash
mosquitto_pub -h localhost -t "device/ESP32-IOT-ACTUATOR-4e5c/gpio/set" -m '{"type":"gpio","pin":3,"state":false}'
```

### Reboot Device
```bash
mosquitto_pub -h localhost -t "device/ESP32-IOT-ACTUATOR-4e5c/cmd" -m '{"cmd":"reboot"}'
```

### Subscribe to Status
```bash
mosquitto_sub -h localhost -t "devices/ESP32-IOT-ACTUATOR-4e5c/status"
```

### Subscribe to Telemetry
```bash
mosquitto_sub -h localhost -t "devices/ESP32-IOT-ACTUATOR-4e5c/telemetry"
```

## Node.js Example

```javascript
const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://localhost:1883');

const deviceId = 'ESP32-IOT-ACTUATOR-4e5c';

client.on('connect', () => {
  console.log('Connected to MQTT broker');
  
  // Subscribe to device telemetry
  client.subscribe(`devices/${deviceId}/telemetry`);
  client.subscribe(`devices/${deviceId}/status`);
  
  // Turn GPIO 1 ON
  client.publish(`device/${deviceId}/gpio/set`, JSON.stringify({
    type: 'gpio',
    pin: 1,
    state: true
  }));
});

client.on('message', (topic, message) => {
  console.log(`${topic}: ${message.toString()}`);
  const data = JSON.parse(message.toString());
  
  if (topic.endsWith('/telemetry')) {
    console.log('GPIO States:', data.gpio);
  }
});
```

## Important Notes

⚠️ **Topic Naming Convention:**
- **Publishing topics** use `devices/` (plural)
- **Subscribing topics** use `device/` (singular)

📌 **Device Discovery:**
- Status messages are retained on the broker
- Telemetry published every 10 seconds
- All GPIO states included in telemetry

🔒 **Security:**
- Currently no authentication required
- Can be extended with MQTT username/password
- Pairing token feature available in web interface
