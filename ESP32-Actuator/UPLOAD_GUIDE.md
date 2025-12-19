# How to Upload Actuator Firmware to a Specific COM Port

## Quick Reference

### Option 1: Command Line (Fastest)
```powershell
# Upload to specific COM port
platformio run --target upload --upload-port COM8

# Upload and monitor on specific port
platformio run --target upload --upload-port COM8 && platformio device monitor --port COM8
```

### Option 2: Modify platformio.ini
Add these lines to your environment:
```ini
[env:esp32-s3-actuator]
upload_port = COM8
monitor_port = COM8
```

### Option 3: Create Multiple Environments
For managing multiple ESP32 devices:
```ini
[env:sensor-device]
platform = espressif32
board = esp32-s3-devkitc-1
upload_port = COM7
build_src_filter = +<*> -<actuator/>

[env:actuator-device]
platform = espressif32
board = esp32-s3-devkitc-1
upload_port = COM8
build_src_filter = +<actuator/*> -<*.cpp>
```

Then upload with:
```powershell
# Upload to sensor on COM7
platformio run -e sensor-device --target upload

# Upload to actuator on COM8
platformio run -e actuator-device --target upload
```

---

## Step-by-Step Guide for Actuator

### Step 1: Find Available COM Ports
```powershell
# List all COM ports
platformio device list

# Or in PowerShell
Get-WmiObject Win32_SerialPort | Select-Object DeviceID,Description
```

Example output:
```
COM7 - USB Serial Device (COM7)
COM8 - USB Serial Device (COM8)
```

### Step 2: Create Actuator Project

#### Method A: Separate Project (Recommended)
```powershell
# 1. Create new directory
mkdir ..\FinalProject_ESP32_Actuator
cd ..\FinalProject_ESP32_Actuator

# 2. Copy actuator files
Copy-Item -Path "..\FinalProject_ESP32_RaspberryPi_LocalMQTT\src\actuator\*" -Destination "src\" -Recurse
Copy-Item -Path "..\FinalProject_ESP32_RaspberryPi_LocalMQTT\src\actuator\include\*" -Destination "include\" -Recurse
Copy-Item -Path "..\FinalProject_ESP32_RaspberryPi_LocalMQTT\src\actuator\platformio.ini" -Destination "platformio.ini"

# 3. Edit platformio.ini and set upload port
# Add line: upload_port = COM8

# 4. Build and upload
platformio run --target upload
```

#### Method B: Use Current Project
In the current project, you can't directly compile actuator code because PlatformIO will compile both sensor and actuator together (causing conflicts).

**Solution:** Use build filters in platformio.ini:
```ini
[env:sensor]
build_src_filter = +<*> -<actuator/>
upload_port = COM7

[env:actuator]  
build_src_filter = +<actuator/*> -<*.cpp>
upload_port = COM8
lib_deps = 
    knolleary/PubSubClient@^2.8
    bblanchon/ArduinoJson@^7.0.3
    adafruit/Adafruit NeoPixel@^1.12.0
```

### Step 3: Upload to Specific Port

```powershell
# Upload actuator to COM8
platformio run -e actuator --target upload --upload-port COM8

# Or if you set upload_port in platformio.ini:
platformio run -e actuator --target upload
```

### Step 4: Monitor Serial Output
```powershell
# Monitor COM8
platformio device monitor --port COM8 --baud 115200

# Or use environment settings
platformio device monitor -e actuator
```

---

## Common Issues

### Issue 1: "Could not open port COM8"
**Cause:** Port already in use or wrong COM number

**Solution:**
1. Close any serial monitors
2. Check device manager for actual COM port
3. Try unplugging and replugging USB

```powershell
# Close all COM port connections
Get-Process | Where-Object {$_.ProcessName -like "*serial*" -or $_.ProcessName -like "*monitor*"} | Stop-Process
```

### Issue 2: "Upload failed"
**Cause:** ESP32 not in bootloader mode

**Solution:**
1. Hold BOOT button on ESP32
2. Press RESET button
3. Release RESET (keep holding BOOT)
4. Start upload
5. Release BOOT when uploading starts

### Issue 3: Multiple ESP32 devices, wrong one gets flashed
**Solution:** Use specific COM ports

```powershell
# Device 1 (Sensor) on COM7
platformio run --target upload --upload-port COM7

# Device 2 (Actuator) on COM8
platformio run --target upload --upload-port COM8
```

---

## Example Workflow: Two ESP32 Devices

### Device 1: Sensor (COM7)
```powershell
cd FinalProject_ESP32_RaspberryPi_LocalMQTT
platformio run --target upload --upload-port COM7
```

### Device 2: Actuator (COM8)
```powershell
cd FinalProject_ESP32_Actuator
platformio run --target upload --upload-port COM8
```

### Monitor Both
```powershell
# Terminal 1
platformio device monitor --port COM7 --baud 115200

# Terminal 2
platformio device monitor --port COM8 --baud 115200
```

---

## Quick Commands Reference

```powershell
# List devices
platformio device list

# Upload to COM8
platformio run --target upload --upload-port COM8

# Monitor COM8
platformio device monitor --port COM8

# Clean build and upload to COM8
platformio run --target clean
platformio run --target upload --upload-port COM8

# Upload and immediately monitor
platformio run --target upload --upload-port COM8; platformio device monitor --port COM8
```

---

## VS Code Tasks (Optional)

Add to `.vscode/tasks.json`:
```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Upload Actuator (COM8)",
      "type": "shell",
      "command": "platformio",
      "args": ["run", "--target", "upload", "--upload-port", "COM8"],
      "group": "build"
    },
    {
      "label": "Upload Sensor (COM7)",
      "type": "shell",
      "command": "platformio",
      "args": ["run", "--target", "upload", "--upload-port", "COM7"],
      "group": "build"
    }
  ]
}
```

Then: **Ctrl+Shift+P** → **Tasks: Run Task** → Select task
