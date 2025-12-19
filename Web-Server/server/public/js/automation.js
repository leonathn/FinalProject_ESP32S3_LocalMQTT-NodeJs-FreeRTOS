/**
 * Automation Rules Engine
 */

const automationRules = [];
let automationInterval = null;

function startAutomationEngine() {
  if (automationInterval) clearInterval(automationInterval);
  
  automationInterval = setInterval(checkAutomationRules, 2000);
  addEvent('info', 'Automation engine started');
}

function stopAutomationEngine() {
  if (automationInterval) {
    clearInterval(automationInterval);
    automationInterval = null;
    addEvent('info', 'Automation engine stopped');
  }
}

function checkAutomationRules() {
  const enabledRules = automationRules.filter(rule => rule.enabled);
  
  enabledRules.forEach(rule => {
    const device = getDevice(rule.condition.deviceId);
    
    if (!device || !device.online) return;
    
    const conditionMet = evaluateCondition(rule.condition, device);
    const previousState = rule.lastState || false;
    rule.lastState = conditionMet;
    
    if (conditionMet && !previousState) {
      executeAutomationAction(rule);
    }
    
    if (!conditionMet && previousState && rule.autoToggle) {
      const offAction = { ...rule.action, state: 0 };
      executeAction(offAction);
      addEvent('info', `Auto-toggle OFF: ${rule.name}`);
    }
  });
}

function evaluateCondition(condition, device) {
  const telemetry = device.telemetry || {};
  const value = telemetry[condition.parameter];
  
  if (value === undefined) return false;
  
  const threshold = parseFloat(condition.threshold);
  
  switch (condition.operator) {
    case '>': return value > threshold;
    case '<': return value < threshold;
    case '>=': return value >= threshold;
    case '<=': return value <= threshold;
    case '==': return Math.abs(value - threshold) < 0.01;
    default: return false;
  }
}

function executeAutomationAction(rule) {
  executeAction(rule.action);
  addEvent('success', `Automation: ${rule.name}`, {
    condition: rule.condition,
    action: rule.action
  });
}

function executeAction(action) {
  const targetDevice = getDevice(action.deviceId);
  
  if (!targetDevice || !targetDevice.online) return;
  
  controlGPIO(action.deviceId, action.gpio, action.state);
}

function addAutomationRule(rule) {
  rule.id = Date.now() + Math.random();
  rule.enabled = true;
  rule.lastState = false;
  automationRules.push(rule);
  
  renderAutomationRules();
  addEvent('success', 'Automation rule added', rule);
}

function removeAutomationRule(ruleId) {
  const index = automationRules.findIndex(r => r.id === ruleId);
  if (index !== -1) {
    automationRules.splice(index, 1);
    renderAutomationRules();
    addEvent('info', 'Automation rule removed');
  }
}

function toggleAutomationRule(ruleId) {
  const rule = automationRules.find(r => r.id === ruleId);
  if (rule) {
    rule.enabled = !rule.enabled;
    renderAutomationRules();
    addEvent('info', `Automation ${rule.enabled ? 'enabled' : 'disabled'}: ${rule.name}`);
  }
}

function renderAutomationRules() {
  const container = document.getElementById('automationRulesList');
  if (!container) return;
  
  if (automationRules.length === 0) {
    container.innerHTML = '<div class="text-center text-muted">No automation rules configured</div>';
    return;
  }
  
  container.innerHTML = automationRules.map(rule => {
    const condDevice = getDevice(rule.condition.deviceId);
    const actionDevice = getDevice(rule.action.deviceId);
    
    const condDeviceName = condDevice ? condDevice.name : rule.condition.deviceId;
    const actionDeviceName = actionDevice ? actionDevice.name : rule.action.deviceId;
    
    const paramName = getParameterName(rule.condition.parameter);
    
    return `
      <div class="card" style="margin-bottom: 1rem;">
        <div class="flex-between mb-sm">
          <h3 style="margin: 0; font-size: 1.125rem;">${rule.name}</h3>
          <div class="flex gap-sm">
            <label class="toggle-switch">
              <input type="checkbox" ${rule.enabled ? 'checked' : ''} 
                     onchange="toggleAutomationRule(${rule.id})">
              <span class="toggle-slider"></span>
            </label>
            <button class="btn btn-danger btn-sm" onclick="removeAutomationRule(${rule.id})">
              Delete
            </button>
          </div>
        </div>
        <div style="color: var(--text-secondary); font-size: 0.875rem;">
          <strong>IF</strong> ${condDeviceName} ${paramName} ${rule.condition.operator} ${rule.condition.threshold}<br>
          <strong>THEN</strong> ${actionDeviceName} GPIO${rule.action.gpio} = ${rule.action.state ? 'ON' : 'OFF'}
          ${rule.autoToggle ? ' <span class="badge badge-info" style="margin-left: 0.5rem;">Auto-toggle</span>' : ''}
        </div>
        ${rule.lastState ? '<div class="mt-sm"><span class="badge badge-success">● Active</span></div>' : ''}
      </div>
    `;
  }).join('');
}

function getParameterName(param) {
  const names = {
    tC: 'Temperature',
    rh: 'Humidity',
    heap: 'Free Memory',
    uptime: 'Uptime',
    rssi: 'Signal'
  };
  return names[param] || param;
}

function saveAutomationRule() {
  const name = document.getElementById('ruleName').value;
  const condDeviceId = document.getElementById('conditionDevice').value;
  const parameter = document.getElementById('conditionParameter').value;
  const operator = document.getElementById('conditionOperator').value;
  const threshold = document.getElementById('conditionThreshold').value;
  const actionDeviceId = document.getElementById('actionDevice').value;
  const gpio = document.getElementById('actionGpio').value;
  const state = document.getElementById('actionState').value;
  const autoToggle = document.getElementById('autoToggle').checked;
  
  if (!name || !condDeviceId || !parameter || !operator || !threshold || !actionDeviceId || !gpio || state === '') {
    addEvent('error', 'Please fill all required fields');
    return;
  }
  
  const rule = {
    name,
    condition: { deviceId: condDeviceId, parameter, operator, threshold: parseFloat(threshold) },
    action: { deviceId: actionDeviceId, gpio: parseInt(gpio), state: parseInt(state) },
    autoToggle
  };
  
  addAutomationRule(rule);
  closeModal('automationModal');
  document.getElementById('automationForm').reset();
}
