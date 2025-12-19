/**
 * UI Management Module
 */

function initializeDashboard() {
  initMQTT();
  startAutomationEngine();
  switchTab('dashboard');
  setupEventListeners();
  addEvent('info', 'ESP32 IoT Dashboard initialized');
}

function setupEventListeners() {
  const clearEventsBtn = document.getElementById('clearEventsBtn');
  if (clearEventsBtn) clearEventsBtn.addEventListener('click', clearEvents);
  
  const exportEventsBtn = document.getElementById('exportEventsBtn');
  if (exportEventsBtn) exportEventsBtn.addEventListener('click', exportEvents);
}

function switchTab(pageId) {
  document.querySelectorAll('.page').forEach(page => page.classList.remove('active'));
  document.querySelectorAll('.nav-tab').forEach(tab => tab.classList.remove('active'));
  
  const page = document.getElementById(pageId + 'Page');
  if (page) page.classList.add('active');
  
  const tab = document.querySelector(`[onclick="switchTab('${pageId}')"]`);
  if (tab) tab.classList.add('active');
  
  if (pageId === 'devices') {
    renderDeviceGrid();
  } else if (pageId === 'gestures') {
    renderGestureRules();
  } else if (pageId === 'automation') {
    renderAutomationRules();
    populateDeviceSelects();
  } else if (pageId === 'events') {
    updateEventLog();
  }
}

function openModal(modalId) {
  const modal = document.getElementById(modalId);
  if (modal) {
    modal.classList.add('active');
    
    if (modalId === 'automationModal') {
      populateDeviceSelects();
    }
    
    if (modalId === 'gestureModal') {
      populateGestureDeviceSelect();
    }
  }
}

function closeModal(modalId) {
  const modal = document.getElementById(modalId);
  if (modal) modal.classList.remove('active');
}

function populateDeviceSelects() {
  const devices = getAllDevices();
  
  const condDeviceSelect = document.getElementById('conditionDevice');
  if (condDeviceSelect) {
    const sensorDevices = devices.filter(d => d.type === 'sensor');
    condDeviceSelect.innerHTML = sensorDevices.length > 0
      ? sensorDevices.map(d => `<option value="${d.id}">${d.name}</option>`).join('')
      : '<option value="">No sensor devices available</option>';
  }
  
  const actionDeviceSelect = document.getElementById('actionDevice');
  if (actionDeviceSelect) {
    const actuatorDevices = devices.filter(d => d.type === 'actuator');
    actionDeviceSelect.innerHTML = actuatorDevices.length > 0
      ? actuatorDevices.map(d => `<option value="${d.id}">${d.name}</option>`).join('')
      : '<option value="">No actuator devices available</option>';
  }
}

function populateGestureDeviceSelect() {
  const devices = getAllDevices();
  const actuatorDevices = devices.filter(d => d.type === 'actuator');
  
  const select = document.getElementById('gestureDevice');
  if (select) {
    select.innerHTML = actuatorDevices.length > 0
      ? actuatorDevices.map(d => `<option value="${d.id}">${d.name}</option>`).join('')
      : '<option value="">No actuator devices available</option>';
  }
}

function saveGestureRule() {
  const gesture = document.getElementById('gestureType').value;
  const deviceId = document.getElementById('gestureDevice').value;
  const gpio = document.getElementById('gestureGpio').value;
  const state = document.getElementById('gestureState').value;
  
  if (!gesture || !deviceId || !gpio || !state) {
    addEvent('error', 'Please fill all required fields');
    return;
  }
  
  addGestureRule({
    gesture,
    deviceId,
    gpio: parseInt(gpio),
    action: state === '1' ? 'on' : 'off'
  });
  
  // Hide form after saving
  toggleGestureForm();
  document.getElementById('gestureForm').reset();
  addEvent('success', `Gesture rule added: ${gesture} → GPIO ${gpio}`);
}

function toggleGestureForm() {
  const formCard = document.getElementById('gestureFormCard');
  const btn = document.getElementById('toggleGestureFormBtn');
  
  if (formCard.style.display === 'none') {
    formCard.style.display = 'block';
    btn.textContent = '➖ Hide Form';
    btn.classList.remove('btn-primary');
    btn.classList.add('btn-secondary');
    populateGestureDeviceSelect();
  } else {
    formCard.style.display = 'none';
    btn.textContent = '+ Add Rule';
    btn.classList.remove('btn-secondary');
    btn.classList.add('btn-primary');
  }
}

function toggleAutomationForm() {
  const formCard = document.getElementById('automationFormCard');
  const btn = document.getElementById('toggleAutomationFormBtn');
  
  if (formCard.style.display === 'none') {
    formCard.style.display = 'block';
    btn.textContent = '➖ Hide';
    populateDeviceSelects();
  } else {
    formCard.style.display = 'none';
    btn.textContent = '➕ Show';
  }
}

function refreshDevices() {
  updateDashboard();
  addEvent('info', 'Device list refreshed');
}

document.addEventListener('DOMContentLoaded', initializeDashboard);
