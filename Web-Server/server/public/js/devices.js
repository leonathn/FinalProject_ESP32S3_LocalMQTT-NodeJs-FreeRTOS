/**
 * Device Management Module
 * Updated to support 8 GPIO pins per actuator device
 */

const devices = new Map();
const DEVICE_TIMEOUT = 60000;

function handleDeviceMessage(deviceId, telemetry) {
  if (!devices.has(deviceId)) {
    registerNewDevice(deviceId, telemetry);
  }
  
  const device = devices.get(deviceId);
  device.telemetry = telemetry || {};
  device.lastSeen = Date.now();
  device.online = true;
  
  updateDashboard();
  
  if (telemetry && telemetry.tC !== undefined) {
    addEvent('info', `Telemetry: ${deviceId}`, {
      temp: `${telemetry.tC}°C`,
      humidity: `${telemetry.rh}%`
    });
  }
}

function registerNewDevice(deviceId, telemetry) {
  const deviceType = deviceId.includes('ACTUATOR') ? 'actuator' : 'sensor';
  
  const device = {
    id: deviceId,
    name: deviceId.replace('ESP32-IOT-', ''),
    type: deviceType,
    telemetry: telemetry,
    online: true,
    firstSeen: Date.now(),
    lastSeen: Date.now()
  };
  
  devices.set(deviceId, device);
  addEvent('success', `New ${deviceType} connected: ${deviceId}`, { type: deviceType });
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
    
    return `
      <div class="device-card">
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
            ${[1, 2, 3, 4, 5, 6, 7, 8].map(gpio => `
              <div class="gpio-item">
                <div class="gpio-label">GPIO ${gpio}</div>
                <div class="gpio-buttons">
                  <button class="gpio-btn on" onclick="controlGPIO('${device.id}', ${gpio}, 1)">ON</button>
                  <button class="gpio-btn off" onclick="controlGPIO('${device.id}', ${gpio}, 0)">OFF</button>
                </div>
              </div>
            `).join('')}
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

function controlGPIO(deviceId, gpio, state) {
  const device = devices.get(deviceId);
  
  if (!device) {
    addEvent('error', `Device not found: ${deviceId}`);
    return;
  }
  
  if (!device.online) {
    addEvent('warning', `Device offline: ${deviceId}`);
    return;
  }
  
  const topic = `device/${deviceId}/gpio/set`;
  const payload = {
    type: "gpio",
    pin: parseInt(gpio),
    state: state === 1 ? true : false
  };
  
  if (publishMQTT(topic, payload)) {
    addEvent('success', `GPIO ${gpio} → ${state ? 'ON' : 'OFF'} on ${device.name}`);
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
