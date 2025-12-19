/**
 * ESP32 IoT Fleet Management System - Server v2.0
 * Production-Ready Dashboard with Enhanced Features
 */

const express = require('express');
const http = require('http');
const aedes = require('aedes')();
const { Server: WebSocketServer } = require('ws');
const path = require('path');

// Configuration
const HTTP_PORT = 3000;
const MQTT_TCP_PORT = 1883;
const DEVICE_TIMEOUT = 60000;

// Device registry
const devices = new Map();

// Initialize Express
const app = express();
const server = http.createServer(app);

// Middleware
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

// MQTT WebSocket server
const wss = new WebSocketServer({ server, path: '/mqtt' });

wss.on('connection', (ws) => {
  const stream = require('websocket-stream')(ws);
  aedes.handle(stream);
});

// MQTT TCP server
const mqttServer = require('net').createServer(aedes.handle);
mqttServer.listen(MQTT_TCP_PORT, () => {
  console.log('[MQTT] TCP broker listening on port', MQTT_TCP_PORT);
});

// MQTT Events
aedes.on('client', (client) => {
  console.log('[MQTT] Client connected:', client.id);
});

aedes.on('clientDisconnect', (client) => {
  console.log('[MQTT] Client disconnected:', client.id);
});

aedes.on('publish', (packet, client) => {
  if (!client) return;
  
  const topic = packet.topic;
  const payload = packet.payload.toString();
  
  if (topic.startsWith('devices/') && topic.endsWith('/telemetry')) {
    const deviceId = topic.split('/')[1];
    
    try {
      const telemetry = JSON.parse(payload);
      
      if (!devices.has(deviceId)) {
        devices.set(deviceId, {
          id: deviceId,
          type: deviceId.includes('ACTUATOR') ? 'actuator' : 'sensor',
          firstSeen: Date.now(),
          telemetry: {}
        });
        console.log('[DEVICE] New device registered:', deviceId);
      }
      
      const device = devices.get(deviceId);
      device.lastSeen = Date.now();
      device.telemetry = telemetry;
      
    } catch (error) {
      console.error('[ERROR] Invalid telemetry JSON:', error.message);
    }
  }
  
  if (topic.startsWith('device/') && topic.endsWith('/gpio/set')) {
    const deviceId = topic.split('/')[1];
    console.log('[GPIO] Command sent to', deviceId + ':', payload);
  }
});

// REST API
app.get('/api/devices', (req, res) => {
  const now = Date.now();
  const deviceList = Array.from(devices.values()).map(device => ({
    ...device,
    online: (now - device.lastSeen) < DEVICE_TIMEOUT
  }));
  
  res.json({
    count: deviceList.length,
    devices: deviceList
  });
});

app.post('/api/gpio/control', (req, res) => {
  const { deviceId, gpio, state } = req.body;
  
  if (!deviceId || gpio === undefined || state === undefined) {
    return res.status(400).json({ error: 'Missing required fields' });
  }
  
  const topic = `device/${deviceId}/gpio/set`;
  const payload = JSON.stringify({ gpio: parseInt(gpio), state: parseInt(state) });
  
  aedes.publish({
    topic,
    payload,
    qos: 0,
    retain: false
  }, (error) => {
    if (error) {
      return res.status(500).json({ error: 'MQTT publish failed' });
    }
    
    res.json({ 
      success: true,
      message: `GPIO ${gpio} set to ${state} on ${deviceId}`
    });
  });
});

app.get('/api/health', (req, res) => {
  res.json({
    status: 'healthy',
    uptime: process.uptime(),
    mqtt: {
      clients: Object.keys(aedes.clients).length
    },
    devices: devices.size
  });
});

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start server
server.listen(HTTP_PORT, () => {
  console.log('\n============================================================');
  console.log('ESP32 IoT Fleet Management System - Server v2.0');
  console.log('Production-Ready Dashboard');
  console.log('============================================================');
  console.log('[HTTP] Dashboard:       http://localhost:' + HTTP_PORT);
  console.log('[HTTP] API server:      http://localhost:' + HTTP_PORT + '/api');
  console.log('[MQTT] TCP broker:      mqtt://localhost:' + MQTT_TCP_PORT);
  console.log('[MQTT] WebSocket:       ws://localhost:' + HTTP_PORT + '/mqtt');
  console.log('============================================================');
  console.log('[INFO] Open http://localhost:' + HTTP_PORT + ' in your browser');
  console.log('============================================================\n');
});

// Graceful shutdown
process.on('SIGINT', () => {
  console.log('\n[INFO] Shutting down server...');
  server.close(() => {
    mqttServer.close(() => {
      console.log('[INFO] Server stopped');
      process.exit(0);
    });
  });
});
