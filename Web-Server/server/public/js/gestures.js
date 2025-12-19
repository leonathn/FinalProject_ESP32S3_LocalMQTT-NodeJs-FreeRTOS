/**
 * Gesture Recognition Module  
 * MediaPipe Hands - 5 gestures
 */

let cameraActive = false;
let hands = null;
let camera = null;
const gestureRules = [];
let lastGestureTime = 0;
const GESTURE_COOLDOWN = 2000;
const MIN_CONFIDENCE = 0.85;

async function toggleCamera() {
  const video = document.getElementById('gestureVideo');
  const canvas = document.getElementById('gestureCanvas');
  const btn = document.getElementById('toggleCameraBtn');
  const placeholder = document.querySelector('.camera-placeholder');
  
  if (!cameraActive) {
    try {
      await initializeGestureRecognition();
      
      // Hide placeholder, show canvas
      if (placeholder) placeholder.style.display = 'none';
      canvas.style.display = 'block';
      
      btn.textContent = '🛑 Stop Camera';
      btn.classList.remove('btn-primary');
      btn.classList.add('btn-danger');
      cameraActive = true;
      
      addEvent('success', 'Gesture recognition started');
    } catch (error) {
      addEvent('error', 'Failed to start camera', { error: error.message });
    }
  } else {
    if (camera) camera.stop();
    
    const stream = video.srcObject;
    if (stream) {
      stream.getTracks().forEach(track => track.stop());
    }
    
    video.srcObject = null;
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Show placeholder, hide canvas
    if (placeholder) placeholder.style.display = 'block';
    canvas.style.display = 'none';
    
    btn.textContent = '📷 Start Camera';
    btn.classList.remove('btn-danger');
    btn.classList.add('btn-primary');
    cameraActive = false;
    
    addEvent('info', 'Gesture recognition stopped');
  }
}

async function initializeGestureRecognition() {
  const video = document.getElementById('gestureVideo');
  
  hands = new Hands({
    locateFile: (file) => `https://cdn.jsdelivr.net/npm/@mediapipe/hands/${file}`
  });
  
  hands.setOptions({
    maxNumHands: 1,
    modelComplexity: 1,
    minDetectionConfidence: 0.7,
    minTrackingConfidence: 0.7
  });
  
  hands.onResults(onHandsResults);
  
  camera = new Camera(video, {
    onFrame: async () => {
      await hands.send({ image: video });
    },
    width: 640,
    height: 480
  });
  
  await camera.start();
}

function onHandsResults(results) {
  const canvas = document.getElementById('gestureCanvas');
  const ctx = canvas.getContext('2d');
  
  ctx.save();
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.drawImage(results.image, 0, 0, canvas.width, canvas.height);
  
  if (results.multiHandLandmarks && results.multiHandLandmarks.length > 0) {
    const landmarks = results.multiHandLandmarks[0];
    
    drawConnectors(ctx, landmarks, HAND_CONNECTIONS, { color: '#00d4d4', lineWidth: 2 });
    drawLandmarks(ctx, landmarks, { color: '#10b981', lineWidth: 1, radius: 3 });
    
    const gesture = detectGestureFromLandmarks(landmarks);
    
    if (gesture && gesture.confidence >= MIN_CONFIDENCE) {
      ctx.fillStyle = '#00d4d4';
      ctx.font = 'bold 24px Inter, sans-serif';
      ctx.fillText(`${gesture.name} (${Math.round(gesture.confidence * 100)}%)`, 10, 30);
      
      // Update result card
      updateGestureResultCard(gesture.name, gesture.confidence);
      
      const now = Date.now();
      if (now - lastGestureTime > GESTURE_COOLDOWN) {
        checkGestureRules(gesture.name);
        lastGestureTime = now;
      }
    } else {
      // No confident gesture detected
      updateGestureResultCard(null, 0);
    }
  }
  
  ctx.restore();
}

function updateGestureResultCard(gestureName, confidence) {
  const iconMap = {
    'palm': '✋',
    'fist': '✊',
    'thumbs_up': '👍',
    'point_up': '☝️',
    'victory': '✌️'
  };
  
  const nameMap = {
    'palm': 'Palm',
    'fist': 'Fist',
    'thumbs_up': 'Thumbs Up',
    'point_up': 'Point Up',
    'victory': 'Victory'
  };
  
  const iconEl = document.getElementById('detectedGestureIcon');
  const nameEl = document.getElementById('detectedGestureName');
  const confEl = document.getElementById('detectedGestureConfidence');
  const confValue = document.getElementById('confidenceValue');
  
  if (!iconEl || !nameEl || !confEl || !confValue) return;
  
  if (gestureName && confidence > 0) {
    iconEl.textContent = iconMap[gestureName] || '👋';
    nameEl.textContent = nameMap[gestureName] || 'Unknown';
    nameEl.style.color = 'var(--primary)';
    confEl.textContent = 'Gesture detected!';
    confEl.style.color = 'var(--success)';
    confValue.textContent = Math.round(confidence * 100) + '%';
    confValue.style.color = confidence >= 0.9 ? 'var(--success)' : 'var(--warning)';
  } else {
    iconEl.textContent = '👋';
    nameEl.textContent = 'No Gesture';
    nameEl.style.color = 'var(--text-muted)';
    confEl.textContent = 'Waiting for detection...';
    confEl.style.color = 'var(--text-muted)';
    confValue.textContent = '0%';
    confValue.style.color = 'var(--text-muted)';
  }
}

function detectGestureFromLandmarks(landmarks) {
  const fingers = {
    thumb: landmarks[4].x < landmarks[3].x,
    index: landmarks[8].y < landmarks[6].y,
    middle: landmarks[12].y < landmarks[10].y,
    ring: landmarks[16].y < landmarks[14].y,
    pinky: landmarks[20].y < landmarks[18].y
  };
  
  const extendedCount = Object.values(fingers).filter(Boolean).length;
  
  if (extendedCount === 5) {
    return { name: 'palm', confidence: 0.95 };
  }
  
  if (extendedCount === 0) {
    return { name: 'fist', confidence: 0.95 };
  }
  
  if (fingers.thumb && !fingers.index && !fingers.middle && !fingers.ring && !fingers.pinky) {
    const thumbTip = landmarks[4];
    const indexBase = landmarks[5];
    if (thumbTip.y < indexBase.y) {
      return { name: 'thumbs_up', confidence: 0.90 };
    }
  }
  
  if (!fingers.thumb && fingers.index && !fingers.middle && !fingers.ring && !fingers.pinky) {
    return { name: 'point_up', confidence: 0.90 };
  }
  
  if (fingers.index && fingers.middle && !fingers.ring && !fingers.pinky) {
    return { name: 'victory', confidence: 0.90 };
  }
  
  return null;
}

function checkGestureRules(gestureName) {
  const matchingRules = gestureRules.filter(rule => rule.enabled && rule.gesture === gestureName);
  
  matchingRules.forEach(rule => {
    const device = getDevice(rule.deviceId);
    
    if (!device || !device.online) return;
    
    const state = rule.action === 'on' ? 1 : 0;
    controlGPIO(rule.deviceId, rule.gpio, state);
    
    addEvent('success', `Gesture: ${gestureName} → GPIO${rule.gpio} ${rule.action.toUpperCase()}`, {
      device: device.name
    });
  });
}

function addGestureRule(rule) {
  rule.id = Date.now() + Math.random();
  rule.enabled = true;
  gestureRules.push(rule);
  renderGestureRules();
  addEvent('success', 'Gesture rule added', rule);
}

function removeGestureRule(ruleId) {
  const index = gestureRules.findIndex(r => r.id === ruleId);
  if (index !== -1) {
    gestureRules.splice(index, 1);
    renderGestureRules();
    addEvent('info', 'Gesture rule removed');
  }
}

function toggleGestureRule(ruleId) {
  const rule = gestureRules.find(r => r.id === ruleId);
  if (rule) {
    rule.enabled = !rule.enabled;
    renderGestureRules();
  }
}

function renderGestureRules() {
  const container = document.getElementById('gestureRulesList');
  if (!container) return;
  
  if (gestureRules.length === 0) {
    container.innerHTML = '<div class="text-center text-muted">No gesture rules configured</div>';
    return;
  }
  
  container.innerHTML = gestureRules.map(rule => {
    const device = getDevice(rule.deviceId);
    const deviceName = device ? device.name : rule.deviceId;
    
    return `
      <div class="card" style="margin-bottom: 1rem;">
        <div class="flex-between">
          <div>
            <strong>${getGestureIcon(rule.gesture)} ${rule.gesture}</strong> → 
            ${deviceName} GPIO${rule.gpio} = ${rule.action.toUpperCase()}
          </div>
          <div class="flex gap-sm">
            <label class="toggle-switch">
              <input type="checkbox" ${rule.enabled ? 'checked' : ''} 
                     onchange="toggleGestureRule(${rule.id})">
              <span class="toggle-slider"></span>
            </label>
            <button class="btn btn-danger btn-sm" onclick="removeGestureRule(${rule.id})">
              Delete
            </button>
          </div>
        </div>
      </div>
    `;
  }).join('');
}

function getGestureIcon(gestureName) {
  const icons = {
    palm: '✋',
    fist: '✊',
    thumbs_up: '👍',
    point_up: '☝️',
    victory: '✌️'
  };
  return icons[gestureName] || '👋';
}
