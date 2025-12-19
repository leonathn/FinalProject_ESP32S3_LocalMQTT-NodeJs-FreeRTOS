/*
 * Web Server Implementation
 */

#include "web_server.h"
#include "globals.h"
#include "config_manager.h"
#include "diagnostics.h"
#include <Arduino.h>
#include <ArduinoJson.h>

void setupWebServer() {
  webServer.on("/", handleRoot);
  webServer.on("/save", HTTP_POST, handleSave);
  webServer.on("/api/status", handleStatus);
  webServer.on("/api/diagnostics/run", handleDiagnostics);
  webServer.on("/api/wifi/scan", handleWiFiScan);
  webServer.on("/api/mqtt/scan", handleMQTTScan);
  webServer.on("/api/sensors", handleSensors);
  webServer.on("/api/reset", HTTP_POST, handleReset);
  webServer.begin();
  Serial.println("[Web] Server started on port " + String(WEB_PORT));
}

void handleRoot() {
  String html = R"html(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>IoT System Using MQTT Telemetry - ESP32 Setup</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Arial, sans-serif; background: linear-gradient(135deg, #16a085 0%, #0891b2 100%); min-height: 100vh; padding: 10px; }
    .container { max-width: 600px; margin: 0 auto; background: hsl(222, 47%, 11%); border-radius: 10px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); border: 1px solid hsl(217, 33%, 17%); }
    h1 { color: hsl(210, 20%, 98%); margin-bottom: 5px; font-size: clamp(1.3rem, 4vw, 1.8rem); }
    .subtitle { color: hsl(215, 16%, 75%); margin-bottom: 20px; font-size: clamp(0.8rem, 2.5vw, 0.95rem); word-break: break-all; }
    .subtitle strong { color: hsl(189, 94%, 43%); }
    @media (max-width: 768px) {
      body { padding: 5px; }
      .container { padding: 15px; border-radius: 8px; }
      .tabs { overflow-x: auto; -webkit-overflow-scrolling: touch; }
      .tab { white-space: nowrap; }
    }
    .tabs { display: flex; border-bottom: 2px solid hsl(217, 33%, 17%); margin-bottom: 20px; }
    .tab { padding: 10px 20px; cursor: pointer; border-bottom: 2px solid transparent; margin-bottom: -2px; transition: all 0.2s; color: hsl(215, 16%, 65%); }
    .tab.active { border-bottom-color: hsl(189, 94%, 43%); color: hsl(189, 94%, 43%); font-weight: 600; }
    .tab-content { display: none; }
    .tab-content.active { display: block; }
    .form-group { margin-bottom: 20px; }
    label { display: block; margin-bottom: 5px; font-weight: 600; color: hsl(210, 20%, 98%); }
    input { width: 100%; padding: 10px; border: 2px solid hsl(217, 33%, 17%); border-radius: 6px; font-size: 14px; background: hsl(222, 47%, 15%); color: hsl(210, 20%, 98%); }
    button { width: 100%; padding: 12px; background: hsl(189, 94%, 43%); color: white; border: none; border-radius: 6px; font-size: 16px; font-weight: 600; cursor: pointer; transition: all 0.2s; }
    button:hover { background: hsl(189, 94%, 35%); }
    .info-box { background: hsl(222, 47%, 15%); padding: 15px; border-radius: 6px; margin-bottom: 20px; border: 1px solid hsl(217, 33%, 17%); }
    .status { display: inline-block; padding: 4px 12px; border-radius: 12px; font-size: 12px; font-weight: 600; }
    .status.online { background: hsl(189, 94%, 43%); color: white; }
    .status.offline { background: hsl(0, 84%, 60%); color: white; }
    .diag-item { padding: 10px; background: hsl(222, 47%, 15%); border-radius: 6px; margin-bottom: 10px; display: flex; justify-content: space-between; align-items: center; border: 1px solid hsl(217, 33%, 17%); }
    .pass { color: hsl(189, 94%, 43%); font-weight: 600; }
    .fail { color: hsl(0, 84%, 60%); font-weight: 600; }
    .wifi-item { padding: 12px; background: hsl(222, 47%, 15%); border: 2px solid hsl(217, 33%, 17%); border-radius: 8px; margin-bottom: 8px; cursor: pointer; transition: all 0.2s; display: flex; justify-content: space-between; align-items: center; color: hsl(210, 20%, 98%); }
    .wifi-item:hover { border-color: hsl(189, 94%, 43%); background: hsl(222, 47%, 18%); }
    .wifi-item.selected { border-color: hsl(189, 94%, 43%); background: hsl(189, 94%, 15%); }
    .signal { font-size: 20px; }
    .mqtt-card { background: linear-gradient(135deg, hsl(189, 94%, 43%) 0%, hsl(189, 94%, 35%) 100%); color: white; padding: 20px; border-radius: 10px; margin-bottom: 15px; cursor: pointer; transition: all 0.3s; }
    .mqtt-card:hover { transform: translateY(-2px); box-shadow: 0 8px 20px rgba(16, 185, 194, 0.4); }
    .mqtt-card h3 { margin: 0 0 5px 0; font-size: 18px; }
    .mqtt-card p { margin: 0; opacity: 0.9; font-size: 13px; }
    .manual-card { background: hsl(222, 47%, 15%); border: 2px dashed hsl(217, 33%, 17%); color: hsl(210, 20%, 98%); }
    .manual-card:hover { border-color: hsl(189, 94%, 43%); }
  </style>
</head>
<body>
  <div class="container">
    <h1>📡 IoT System Using MQTT Telemetry</h1>
    <p class="subtitle">Device ID: <strong>)html" + deviceId + R"html(</strong></p>
    
    <div class="tabs">
      <div class="tab active" data-tab="wifi">WiFi</div>
      <div class="tab" data-tab="mqtt">MQTT Broker</div>
      <div class="tab" data-tab="sensors">Sensors</div>
    </div>
    
    <div id="wifi-tab" class="tab-content active">
      <h3 style="margin-bottom: 15px;">WiFi Configuration</h3>
      <button id="scanBtn" style="margin-bottom: 15px;">🔍 Scan WiFi Networks</button>
      <div id="wifi-list" style="margin-bottom: 20px;"></div>
      <form action="/save" method="POST" id="configForm">
        <div class="form-group">
          <label>Selected Network</label>
          <input type="text" name="ssid" id="ssidInput" value="" placeholder="Click a network above or type manually" required>
        </div>
        <div class="form-group">
          <label>WiFi Password</label>
          <input type="password" name="password" id="passwordInput" value="" placeholder="Enter WiFi password">
        </div>
        <input type="hidden" name="mqtt_server" id="mqttServerInput" value=")html" + mqttServer + R"html(">
        <input type="hidden" name="mqtt_port" id="mqttPortInput" value=")html" + String(mqttPort) + R"html(">
        <input type="hidden" name="token" id="tokenInput" value=")html" + pairingToken + R"html(">
        <button type="submit">💾 Save Configuration & Reboot</button>
      </form>
    </div>
    
    <div id="mqtt-tab" class="tab-content">
      <h3 style="margin-bottom: 20px; color: hsl(210, 20%, 98%);">🔧 MQTT Broker Configuration</h3>
      
      <!-- MQTT Broker Scanner -->
      <button id="scanMqttBtn" style="margin-bottom: 15px; background: hsl(189, 94%, 43%);">🔍 Scan for MQTT Brokers</button>
      <div id="mqtt-scan-results" style="margin-bottom: 20px;"></div>
      
      <!-- Info Box -->
      <div style="background: hsl(189, 94%, 15%); padding: 15px; border-radius: 8px; margin-bottom: 20px; border-left: 4px solid hsl(189, 94%, 43%); color: hsl(210, 20%, 98%);">
        <strong>ℹ️ How to find your computer's IP:</strong><br>
        <span style="font-size: 0.9em; color: hsl(215, 16%, 75%);">1. Open PowerShell or CMD<br>2. Type: <code style="background: hsl(222, 47%, 20%); padding: 2px 6px; border-radius: 3px; color: hsl(189, 94%, 60%);">ipconfig</code><br>3. Look for "IPv4 Address" (example: 10.212.0.109)</span>
      </div>

      <!-- Selected Broker Display -->
      <div style="background: hsl(222, 47%, 15%); padding: 15px; border-radius: 8px; margin-bottom: 20px; border: 1px solid hsl(217, 33%, 17%);">
        <div style="font-size: 0.9em; color: hsl(215, 16%, 75%); margin-bottom: 5px;">Selected Broker:</div>
        <div style="font-size: 1.2em; font-weight: 600; color: hsl(189, 94%, 43%);">
          <span id="currentServer">)html" + (mqttServer.length() > 0 ? mqttServer : String("Not configured")) + R"html(</span>:<span id="currentPort">)html" + String(mqttPort) + R"html(</span>
        </div>
        <div style="font-size: 0.85em; color: hsl(215, 16%, 75%); margin-top: 5px;">Device ID: <strong style="color: hsl(189, 94%, 60%);">)html" + deviceId + R"html(</strong></div>
      </div>

      <!-- Manual Entry -->
      <div style="background: hsl(222, 47%, 15%); border: 2px solid hsl(217, 33%, 17%); border-radius: 8px; padding: 15px; margin-bottom: 20px;">
        <label style="font-weight: 600; margin-bottom: 10px; display: block; color: hsl(210, 20%, 98%);">Or Enter Manually:</label>
        <div class="form-group">
          <label>MQTT Server IP/Hostname</label>
          <input type="text" id="manualServer" value="" placeholder="192.168.1.100">
        </div>
        <div class="form-group">
          <label>Port</label>
          <input type="number" id="manualPort" value="" placeholder="1883">
        </div>
      </div>

      <!-- Pairing Token -->
      <div style="background: hsl(189, 94%, 15%); padding: 15px; border-radius: 8px; margin-bottom: 20px; border-left: 4px solid hsl(189, 94%, 43%);">
        <label style="font-weight: 600; margin-bottom: 5px; display: block; color: hsl(210, 20%, 98%);">🔑 Pairing Token (Optional)</label>
        <p style="font-size: 0.85em; color: hsl(215, 16%, 65%); margin: 5px 0 10px 0;">A 6-digit security code to pair this device with your server. Leave blank to use default (123456).</p>
        <input type="text" id="tokenField" value="" placeholder="123456" pattern="[0-9]{6}" maxlength="6">
      </div>

      <!-- Apply Button -->
      <button id="applyMqttBtn" style="width: 100%; background: hsl(189, 94%, 43%); font-size: 1.1em;">✓ Apply MQTT Settings</button>
      <p style="text-align: center; margin-top: 10px; font-size: 0.85em; color: hsl(215, 16%, 65%);">Then go to WiFi tab to save & reboot</p>
    </div>
    
    <div id="sensors-tab" class="tab-content">
      <h3 style="margin-bottom: 15px; color: hsl(210, 20%, 98%);">📊 Live Sensor Readings</h3>
      <div class="info-box">
        <div class="diag-item">
          <span style="color: hsl(210, 20%, 98%);"><strong>🌡️ Temperature</strong></span>
          <span id="temp" style="font-size: 18px; font-weight: 600; color: hsl(189, 94%, 60%);">--</span>
        </div>
        <div class="diag-item">
          <span style="color: hsl(210, 20%, 98%);"><strong>💧 Humidity</strong></span>
          <span id="hum" style="font-size: 18px; font-weight: 600; color: hsl(189, 94%, 60%);">--</span>
        </div>
        <div class="diag-item">
          <span style="color: hsl(215, 16%, 75%);">Sensor Status</span>
          <span id="tempBand" class="status">--</span>
        </div>
        <div class="diag-item">
          <span style="color: hsl(215, 16%, 75%);">Last Update</span>
          <span id="lastUpdate" style="color: hsl(215, 16%, 75%);">--</span>
        </div>
        <div class="diag-item">
          <span style="color: hsl(215, 16%, 75%);">Total Reads</span>
          <span id="readCount" style="color: hsl(215, 16%, 75%);">--</span>
        </div>
      </div>
    </div>
  </div>
  
  <script>
    let autoRefreshInterval = null;
    let selectedSSID = '';
    
    function showTab(tab) {
      // Always clear any existing interval first
      if (autoRefreshInterval) {
        clearInterval(autoRefreshInterval);
        autoRefreshInterval = null;
      }
      
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
      document.querySelector('[data-tab="' + tab + '"]').classList.add('active');
      document.getElementById(tab + '-tab').classList.add('active');
      
      // Start sensor updates only when on sensors tab
      if (tab === 'sensors') {
        updateSensors();
        autoRefreshInterval = setInterval(updateSensors, 3000);
      }
    }
    
    async function scanMQTTBrokers() {
      const btn = document.getElementById('scanMqttBtn');
      const resultsDiv = document.getElementById('mqtt-scan-results');
      
      btn.disabled = true;
      btn.textContent = 'Scanning...';
      resultsDiv.innerHTML = '<div style="color: hsl(215, 16%, 75%); text-align: center;">Scanning local network for MQTT brokers...</div>';
      
      try {
        const response = await fetch('/api/mqtt/scan');
        const data = await response.json();
        
        if (data.brokers && data.brokers.length > 0) {
          resultsDiv.innerHTML = '<div style="color: hsl(215, 16%, 75%); margin-bottom: 10px;">Found ' + data.brokers.length + ' MQTT Broker(s):</div>';
          
          data.brokers.forEach(broker => {
            const card = document.createElement('div');
            card.className = 'mqtt-card';
            card.style.marginBottom = '8px';
            card.innerHTML = '<h3 style="font-size: 16px;">' + broker.hostname + '</h3><div style="color: hsl(215, 16%, 75%); font-size: 14px;">' + broker.ip + ':' + broker.port + '</div>';
            card.setAttribute('data-server', broker.ip);
            card.setAttribute('data-port', broker.port);
            card.addEventListener('click', function() {
              selectMQTT(this.getAttribute('data-server'), this.getAttribute('data-port'));
            });
            resultsDiv.appendChild(card);
          });
        } else {
          resultsDiv.innerHTML = '<div style="color: hsl(33, 100%, 50%); text-align: center;">No MQTT brokers found via mDNS. Using common defaults:</div>';
          
          // Fallback to common IPs
          const commonIPs = ['10.212.0.109', '192.168.1.100', '192.168.0.1'];
          commonIPs.forEach(ip => {
            const card = document.createElement('div');
            card.className = 'mqtt-card';
            card.style.marginBottom = '8px';
            card.innerHTML = '<h3 style="font-size: 16px;">' + ip + ':1883</h3>';
            card.setAttribute('data-server', ip);
            card.setAttribute('data-port', '1883');
            card.addEventListener('click', function() {
              selectMQTT(this.getAttribute('data-server'), this.getAttribute('data-port'));
            });
            resultsDiv.appendChild(card);
          });
        }
      } catch (error) {
        resultsDiv.innerHTML = '<div style="color: hsl(0, 84%, 60%); text-align: center;">Scan failed. Please enter manually.</div>';
      }
      
      btn.disabled = false;
      btn.textContent = '🔍 Scan for MQTT Brokers';
    }
    
    async function scanWiFi() {
      const btn = document.getElementById('scanBtn');
      btn.disabled = true;
      btn.textContent = 'Scanning...';
      
      const res = await fetch('/api/wifi/scan');
      const data = await res.json();
      
      const list = document.getElementById('wifi-list');
      list.innerHTML = '';
      
      if (data.count > 0) {
        const title = document.createElement('div');
        title.style.marginBottom = '10px';
        title.innerHTML = '<strong>Click a network to select:</strong>';
        list.appendChild(title);
        
        data.networks.forEach((net) => {
          const signal = net.rssi > -60 ? '📶📶📶' : net.rssi > -75 ? '📶📶' : '📶';
          const lock = net.encryption === 'Open' ? '🔓' : '🔒';
          
          const item = document.createElement('div');
          item.className = 'wifi-item';
          item.innerHTML = '<span>' + lock + ' <strong>' + net.ssid + '</strong></span><span>' + signal + ' ' + net.rssi + ' dBm</span>';
          item.addEventListener('click', function(e) {
            selectWiFi(net.ssid, e.currentTarget);
          });
          list.appendChild(item);
        });
      } else {
        list.innerHTML = '<div class=\"info-box\">No networks found. Try again.</div>';
      }
      
      btn.disabled = false;
      btn.textContent = '🔍 Scan WiFi Networks';
    }
    
    function selectWiFi(ssid, element) {
      selectedSSID = ssid;
      document.getElementById('ssidInput').value = ssid;
      document.querySelectorAll('.wifi-item').forEach(item => item.classList.remove('selected'));
      element.classList.add('selected');
      document.getElementById('passwordInput').focus();
    }
    
    function selectMQTT(server, port) {
      document.getElementById('manualServer').value = server;
      document.getElementById('manualPort').value = port;
      
      // Visual feedback - highlight selected card briefly
      document.querySelectorAll('.mqtt-card').forEach(card => {
        card.style.opacity = '0.6';
      });
      event.target.closest('.mqtt-card').style.opacity = '1';
      setTimeout(() => {
        document.querySelectorAll('.mqtt-card').forEach(card => {
          card.style.opacity = '1';
        });
      }, 300);
    }
    
    function applyMQTT() {
      const server = document.getElementById('manualServer').value;
      const port = document.getElementById('manualPort').value;
      const token = document.getElementById('tokenField').value;
      
      if (!server || !port) {
        alert('❌ Please enter both MQTT server and port');
        return;
      }
      
      // Validate token (6 digits or empty)
      if (token && (token.length !== 6 || isNaN(token))) {
        alert('❌ Token must be exactly 6 digits');
        return;
      }
      
      // Update hidden form fields
      document.getElementById('mqttServerInput').value = server;
      document.getElementById('mqttPortInput').value = port;
      document.getElementById('tokenInput').value = token || '123456';
      
      // Update display
      document.getElementById('currentServer').textContent = server;
      document.getElementById('currentPort').textContent = port;
      
      alert('✅ MQTT Settings Applied!\\n\\nServer: ' + server + ':' + port + '\\nToken: ' + (token || '123456') + '\\n\\nNow go to WiFi tab and click \"Save Configuration & Reboot\"');
    }
    
    function toggleManualMQTT() {
      // Not needed anymore - manual entry is always visible
    }
    
    function saveManualMQTT() {
      // Deprecated - use applyMQTT instead
      applyMQTT();
    }
    
    async function updateSensors() {
      const res = await fetch('/api/sensors');
      const data = await res.json();
      
      if (data.dht20Ok) {
        document.getElementById('temp').textContent = data.temperature.toFixed(1) + ' °C';
        document.getElementById('hum').textContent = data.humidity.toFixed(1) + ' %';
        document.getElementById('tempBand').textContent = 'OK';
        document.getElementById('tempBand').className = 'status online';
        document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
        if (data.readCount !== undefined) {
          document.getElementById('readCount').textContent = data.readCount;
        }
      } else {
        document.getElementById('temp').textContent = 'Error';
        document.getElementById('hum').textContent = 'Error';
        document.getElementById('tempBand').textContent = 'ERROR';
        document.getElementById('tempBand').className = 'status offline';
      }
    }
    
    // Initialize event listeners when page loads
    document.addEventListener('DOMContentLoaded', function() {
      // Tab click handlers
      document.querySelectorAll('.tab').forEach(tab => {
        tab.addEventListener('click', function() {
          showTab(this.getAttribute('data-tab'));
        });
      });
      
      // WiFi scan button
      document.getElementById('scanBtn').addEventListener('click', scanWiFi);
      
      // MQTT broker scanner button
      document.getElementById('scanMqttBtn').addEventListener('click', scanMQTTBrokers);
      
      // MQTT broker cards
      document.querySelectorAll('.mqtt-card[data-server]').forEach(card => {
        card.addEventListener('click', function() {
          selectMQTT(this.getAttribute('data-server'), this.getAttribute('data-port'));
        });
      });
      
      // Apply MQTT button
      const applyBtn = document.getElementById('applyMqttBtn');
      if (applyBtn) {
        applyBtn.addEventListener('click', applyMQTT);
      }
    });
  </script>
</body>
</html>
  )html";
  
  webServer.send(200, "text/html", html);
}

void handleSave() {
  wifiSSID = webServer.arg("ssid");
  wifiPassword = webServer.arg("password");
  mqttServer = webServer.arg("mqtt_server");
  mqttPort = webServer.arg("mqtt_port").toInt();
  pairingToken = webServer.arg("token");
  
  saveConfig();
  
  String html = "<html><body><h1>Saved! Rebooting...</h1><script>setTimeout(() => window.location='/', 5000);</script></body></html>";
  webServer.send(200, "text/html", html);
  
  delay(2000);
  ESP.restart();
}

void handleStatus() {
  JsonDocument doc;
  doc["mode"] = apMode ? "AP" : "STA";
  doc["wifi"] = wifiConnected;
  doc["mqtt"] = mqttConnected;
  doc["uptime"] = millis() / 1000;
  doc["heap"] = ESP.getFreeHeap();
  if (wifiConnected) {
    doc["ip"] = WiFi.localIP().toString();
    doc["rssi"] = WiFi.RSSI();
  }
  
  char buffer[256];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
}

void handleDiagnostics() {
  runDiagnostics();
  
  JsonDocument doc;
  doc["i2cOk"] = lastDiagnostics.i2cOk;
  doc["i2cDevices"] = lastDiagnostics.i2cDevices;
  doc["dht20Ok"] = lastDiagnostics.dht20Ok;
  doc["temperature"] = lastDiagnostics.temperature;
  doc["humidity"] = lastDiagnostics.humidity;
  doc["neopixelOk"] = lastDiagnostics.neopixelOk;
  doc["errorMsg"] = lastDiagnostics.errorMsg;
  
  char buffer[512];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
}

void handleWiFiScan() {
  Serial.println("[Web] WiFi scan requested");
  
  int networksFound = WiFi.scanNetworks();
  
  JsonDocument doc;
  doc["count"] = networksFound;
  JsonArray networks = doc["networks"].to<JsonArray>();
  
  for (int i = 0; i < networksFound; i++) {
    JsonObject net = networks.add<JsonObject>();
    net["ssid"] = WiFi.SSID(i);
    net["rssi"] = WiFi.RSSI(i);
    net["encryption"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted";
  }
  
  char buffer[2048];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
  
  Serial.println("[Web] WiFi scan complete: " + String(networksFound) + " networks");
}

void handleSensors() {
  JsonDocument doc;
  
  // Read current sensor data with mutex protection
  if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    if (dht20.read()) {
      float temp = dht20.getTemperature();
      float hum = dht20.getHumidity();
      
      if (temp > -10 && temp < 60 && hum >= 0 && hum <= 100) {
        doc["dht20Ok"] = true;
        doc["temperature"] = temp;
        doc["humidity"] = hum;
      } else {
        doc["dht20Ok"] = false;
        doc["error"] = "Invalid sensor readings";
      }
    } else {
      doc["dht20Ok"] = false;
      doc["error"] = "Sensor read failed";
    }
    xSemaphoreGive(i2cMutex);
  } else {
    doc["dht20Ok"] = false;
    doc["error"] = "I2C mutex timeout";
  }
  
  doc["uptime"] = millis() / 1000;
  doc["heap"] = ESP.getFreeHeap();
  doc["readCount"] = sensorState.readCount;
  doc["errorCount"] = sensorState.errorCount;
  
  char buffer[256];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
}

void handleMQTTScan() {
  Serial.println("[Web] MQTT scan requested");
  
  JsonDocument doc;
  JsonArray brokers = doc["brokers"].to<JsonArray>();
  
  // Query mDNS for MQTT services
  int n = MDNS.queryService("mqtt", "tcp");
  
  if (n > 0) {
    Serial.println("[mDNS] Found " + String(n) + " MQTT service(s)");
    for (int i = 0; i < n; i++) {
      JsonObject broker = brokers.add<JsonObject>();
      broker["ip"] = MDNS.IP(i).toString();
      broker["port"] = MDNS.port(i);
      broker["hostname"] = MDNS.hostname(i);
      
      Serial.println("  - " + MDNS.hostname(i) + " @ " + MDNS.IP(i).toString() + ":" + String(MDNS.port(i)));
    }
  } else {
    Serial.println("[mDNS] No MQTT services found");
    // Add fallback to common IP if on WiFi
    if (wifiConnected) {
      IPAddress localIP = WiFi.localIP();
      // Use gateway as likely server location
      IPAddress gateway = WiFi.gatewayIP();
      
      JsonObject broker = brokers.add<JsonObject>();
      broker["ip"] = gateway.toString();
      broker["port"] = 1883;
      broker["hostname"] = "Gateway (guessed)";
    }
  }
  
  char buffer[1024];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
}

void handleReset() {
  Serial.println("[API] Reset configuration requested via web interface");
  
  // Send success response first
  JsonDocument doc;
  doc["success"] = true;
  doc["message"] = "Configuration cleared. Rebooting in 3 seconds...";
  
  char buffer[128];
  serializeJson(doc, buffer);
  webServer.send(200, "application/json", buffer);
  
  // Clear configuration
  prefs.begin("esp32-iot", false);
  prefs.clear();
  prefs.end();
  
  Serial.println("[Config] Cleared! Rebooting in 3 seconds...");
  
  // Wait a bit for response to be sent
  delay(3000);
  
  // Reboot
  ESP.restart();
}
