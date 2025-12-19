/**
 * Device Management Module
 * Updated to support 8 GPIO pins per actuator device
 */

const devices = new Map();
const DEVICE_TIMEOUT = 60000;

// Normalize device ID to prevent duplicates from different formats
function normalizeDeviceId(deviceId) {
  const upper = deviceId.toUpperCase();
  
  // Extract device type (ACTUATOR or SENSOR)
  const type = upper.includes('ACTUATOR') ? 'ACTUATOR' : 'SENSOR';
  
  // Extract last 4 characters (usually the unique ID like 4E5C or 731C)
  const match = upper.match(/([A-F0-9]{4})$/i);
  const suffix = match ? match[1] : upper.slice(-4);
  
  return `${type}-${suffix}`;
}

function handleDeviceMessage(deviceId, telemetry) {
  // Normalize device ID to prevent duplicates
  const normalizedId = normalizeDeviceId(deviceId);
  
  if (!devices.has(normalizedId)) {
    registerNewDevice(deviceId, normalizedId, telemetry);
  } else {
    // Update existing device - make sure originalId is preserved
    const device = devices.get(normalizedId);
    if (!device.originalId) {
      device.originalId = deviceId;
    }
  }
  
  const device = devices.get(normalizedId);
  device.telemetry = telemetry || {};
  device.lastSeen = Date.now();
  device.online = true;
  
  updateDashboard();
  
  if (telemetry && telemetry.tC !== undefined) {
    addEvent('info', `Telemetry: ${device.name}`, {
      temp: `${telemetry.tC}°C`,
      humidity: `${telemetry.rh}%`
    });
  }
}

function registerNewDevice(originalId, normalizedId, telemetry) {
  // Improved device type detection
  const idLower = originalId.toLowerCase();
  const deviceType = idLower.includes('actuator') ? 'actuator' : 'sensor';
  
  const device = {
    id: normalizedId,
    originalId: originalId,  // Keep full original ID for MQTT
    name: originalId.replace('ESP32-IOT-', '').replace('ESP32S3-', ''),
    type: deviceType,
    telemetry: telemetry,
    online: true,
    firstSeen: Date.now(),
    lastSeen: Date.now()
  };
  
  devices.set(normalizedId, device);
  addEvent('success', `New ${deviceType} connected: ${originalId}`, { type: deviceType });
}

function updateDashboard() {
  const now = Date.now();
  
  devices.forEach(device => {
    if (now - device.lastSeen > DEVICE_TIMEOUT) {
      device.online = false;
    }
  });
  
  updateStats();
  renderDeviceGrid();
}

function updateStats() {
  const totalDevices = devices.size;
  const onlineDevices = Array.from(devices.values()).filter(d => d.online).length;
  const sensors = Array.from(devices.values()).filter(d => d.type === 'sensor').length;
  const actuators = Array.from(devices.values()).filter(d => d.type === 'actuator').length;
  
  const totalEl = document.getElementById('totalDevices');
  const onlineEl = document.getElementById('onlineDevices');
  const sensorsEl = document.getElementById('sensorCount');
  const actuatorsEl = document.getElementById('actuatorCount');
  
  if (totalEl) totalEl.textContent = totalDevices;
  if (onlineEl) onlineEl.textContent = onlineDevices;
  if (sensorsEl) sensorsEl.textContent = sensors;
  if (actuatorsEl) actuatorsEl.textContent = actuators;
}

function renderDeviceGrid() {
  const grid = document.getElementById('deviceGrid');
  if (!grid) return;
  
  const deviceArray = Array.from(devices.values());
  
  if (deviceArray.length === 0) {
    grid.innerHTML = '<div class="text-center text-muted" style="grid-column: 1/-1; padding: 3rem;">No devices connected</div>';
    return;
  }
  
  grid.innerHTML = deviceArray.map(device => {
    const isOnline = device.online;
    const telemetry = device.telemetry || {};
    const cardClass = device.type === 'actuator' ? 'device-card device-card-wide' : 'device-card';
    
    return `
      <div class="${cardClass}">
        <div class="device-header">
          <div class="device-info">
            <h3>${device.name}</h3>
            <div class="device-type-badge">${device.type}</div>
          </div>
          <span class="badge ${isOnline ? 'badge-success' : 'badge-danger'}">
            ${isOnline ? '● Online' : '● Offline'}
          </span>
        </div>
        
        ${device.type === 'sensor' && telemetry.tC !== undefined ? `
          <div class="telemetry-grid">
            <div class="telemetry-item">
              <div class="telemetry-icon">🌡️</div>
              <div class="telemetry-label">Temp</div>
              <div class="telemetry-value">${parseFloat(telemetry.tC).toFixed(1)}°C</div>
            </div>
            <div class="telemetry-item">
              <div class="telemetry-icon">💧</div>
              <div class="telemetry-label">Humidity</div>
              <div class="telemetry-value">${parseFloat(telemetry.rh).toFixed(1)}%</div>
            </div>
          </div>
        ` : ''}
        
        ${device.type === 'actuator' ? `
          <div class="gpio-grid">
            ${[1, 2, 3, 4, 5, 6, 7, 8].map(gpio => {
              const pinMap = {1:5, 2:6, 3:7, 4:8, 5:9, 6:10, 7:21, 8:38};
              const boardLabel = `D${gpio + 1}`;
              const isOn = device.gpioStates && device.gpioStates[gpio];
              
              return `
              <div class="gpio-item">
                <span class="gpio-label">${boardLabel}</span>
                <label class="gpio-switch">
                  <input type="checkbox" ${isOn ? 'checked' : ''} 
                         onchange="controlGPIO('${device.id}', ${gpio}, this.checked ? 1 : 0)">
                  <span class="gpio-slider"></span>
                </label>
              </div>
              `;
            }).join('')}
          </div>
        ` : ''}
        
        <div class="device-footer">
          <span>Heap: ${telemetry.heap !== undefined ? formatBytes(telemetry.heap) : 'N/A'}</span>
          <span>Updated: ${isOnline ? 'just now' : new Date(device.lastSeen).toLocaleTimeString()}</span>
        </div>
      </div>
    `;
  }).join('');
}

function controlGPIO(normalizedId, gpio, state) {
  const device = devices.get(normalizedId);
  
  if (!device) {
    addEvent('error', `Device not found: ${normalizedId}`);
    return;
  }
  
  if (!device.online) {
    addEvent('warning', `Device offline: ${device.name}`);
    return;
  }
  
  // Update local state
  if (!device.gpioStates) device.gpioStates = {};
  device.gpioStates[gpio] = state === 1;
  
  // Use the original full device ID for MQTT topic
  const topic = `device/${device.originalId || device.name}/gpio/set`;
  const payload = {
    type: "gpio",
    pin: parseInt(gpio),
    state: state === 1 ? true : false
  };
  
  if (publishMQTT(topic, payload)) {
    addEvent('success', `GPIO ${gpio} → ${state ? 'ON' : 'OFF'} on ${device.name}`);
    updateDashboard();
  }
}

function formatBytes(bytes) {
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';
  return (bytes / 1048576).toFixed(1) + ' MB';
}

function formatUptime(seconds) {
  const hours = Math.floor(seconds / 3600);
  const minutes = Math.floor((seconds % 3600) / 60);
  return `${hours}h ${minutes}m`;
}

function getAllDevices() {
  return Array.from(devices.values());
}

function getDevice(deviceId) {
  return devices.get(deviceId) || null;
}

function getDevicesByType(type) {
  return Array.from(devices.values()).filter(d => d.type === type);
}

setInterval(updateDashboard, 1000);
