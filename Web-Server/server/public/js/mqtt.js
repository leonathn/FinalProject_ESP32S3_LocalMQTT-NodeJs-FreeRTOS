/**
 * MQTT Client Module
 */

let mqttClient = null;
let mqttConnected = false;

function initMQTT() {
  const wsUrl = `ws://${window.location.hostname}:${window.location.port}/mqtt`;
  
  // Display MQTT broker IP
  displayBrokerIP();
  
  // Use persistent client ID stored in sessionStorage to prevent reconnects
  let clientId = sessionStorage.getItem('mqttClientId');
  if (!clientId) {
    clientId = 'dashboard_' + Math.random().toString(16).substr(2, 8);
    sessionStorage.setItem('mqttClientId', clientId);
  }
  
  addEvent('info', 'Connecting to MQTT broker...', { url: wsUrl });
  
  mqttClient = mqtt.connect(wsUrl, {
    clientId: clientId,
    clean: false,
    reconnectPeriod: 5000
  });
  
  mqttClient.on('connect', onMQTTConnect);
  mqttClient.on('message', onMQTTMessage);
  mqttClient.on('error', onMQTTError);
  mqttClient.on('offline', onMQTTOffline);
  mqttClient.on('reconnect', onMQTTReconnect);
}

function onMQTTConnect() {
  mqttConnected = true;
  addEvent('success', 'MQTT broker connected');
  
  updateMQTTStatus('connected');
  
  mqttClient.subscribe('devices/+/status');
  mqttClient.subscribe('devices/+/telemetry');
  mqttClient.subscribe('devices/+/diagnostics');
  mqttClient.subscribe('device/+/status');
  mqttClient.subscribe('gestures/detected');
  
  addEvent('info', 'Subscribed to device topics');
}

function onMQTTMessage(topic, message) {
  try {
    const payload = message.toString();
    
    if (topic.includes('/telemetry')) {
      const deviceId = topic.split('/')[1];
      const data = JSON.parse(payload);
      handleDeviceMessage(deviceId, data);
    }
    else if (topic.includes('/status')) {
      const deviceId = topic.split('/')[1];
      // Try to parse as JSON, fallback to text
      try {
        const data = JSON.parse(payload);
        handleDeviceMessage(deviceId, data);
      } catch {
        // If not JSON, just register the device
        if (!devices.has(deviceId)) {
          registerNewDevice(deviceId, {});
        }
        addEvent('info', `Device status update: ${deviceId}`, { status: payload });
      }
    }
    else if (topic === 'gestures/detected') {
      const data = JSON.parse(payload);
      addEvent('success', `Gesture detected: ${data.gesture}`, data);
    }
    
  } catch (error) {
    addEvent('error', 'Failed to parse MQTT message', { topic, error: error.message });
  }
}

function onMQTTError(error) {
  mqttConnected = false;
  addEvent('error', 'MQTT connection error', { error: error.message });
  updateMQTTStatus('error');
}

function onMQTTOffline() {
  mqttConnected = false;
  addEvent('warning', 'MQTT broker offline');
  updateMQTTStatus('offline');
}

function onMQTTReconnect() {
  addEvent('info', 'Reconnecting to MQTT broker...');
}

function updateMQTTStatus(status) {
  const statusEl = document.getElementById('mqttStatus');
  if (!statusEl) return;
  
  const statusConfig = {
    connected: { text: 'Connected', class: 'connected' },
    offline: { text: 'Offline', class: '' },
    error: { text: 'Error', class: '' }
  };
  
  const config = statusConfig[status] || statusConfig.offline;
  statusEl.innerHTML = `<span class="status-dot ${config.class}"></span> ${config.text}`;
}

function publishMQTT(topic, payload, options = {}) {
  if (!mqttClient || !mqttConnected) {
    addEvent('error', 'MQTT not connected - cannot publish');
    return false;
  }
  
  const message = typeof payload === 'object' ? JSON.stringify(payload) : payload;
  
  mqttClient.publish(topic, message, {
    qos: options.qos || 0,
    retain: options.retain || false
  }, (error) => {
    if (error) {
      addEvent('error', 'MQTT publish failed', { topic, error: error.message });
    }
  });
  
  return true;
}

function isMQTTConnected() {
  return mqttConnected;
}

function disconnectMQTT() {
  if (mqttClient) {
    mqttClient.end();
    mqttConnected = false;
    addEvent('info', 'MQTT disconnected');
  }
}

function displayBrokerIP() {
  const brokerIpEl = document.getElementById('brokerIp');
  if (!brokerIpEl) return;
  
  // Get the actual hostname (could be IP or hostname)
  const hostname = window.location.hostname;
  
  // If it's localhost or 127.0.0.1, try to get the real IP
  if (hostname === 'localhost' || hostname === '127.0.0.1') {
    // Fetch from server API
    fetch('/api/server-ip')
      .then(res => res.json())
      .then(data => {
        if (data.ip) {
          brokerIpEl.textContent = `mqtt://${data.ip}:1883`;
        } else {
          brokerIpEl.textContent = 'mqtt://localhost:1883';
        }
      })
      .catch(() => {
        brokerIpEl.textContent = 'mqtt://localhost:1883';
      });
  } else {
    brokerIpEl.textContent = `mqtt://${hostname}:1883`;
  }
}
