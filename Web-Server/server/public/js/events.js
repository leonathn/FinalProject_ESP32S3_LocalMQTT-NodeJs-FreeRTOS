/**
 * Event Logging System
 */

const MAX_EVENTS = 100;
const events = [];

function addEvent(type, message, data = null) {
  const event = {
    id: Date.now() + Math.random(),
    type,
    message,
    data,
    timestamp: new Date().toISOString()
  };
  
  events.unshift(event);
  
  if (events.length > MAX_EVENTS) {
    events.pop();
  }
  
  updateEventLog();
  console.log(`[${type.toUpperCase()}]`, message, data || '');
}

function updateEventLog() {
  const eventLog = document.getElementById('eventLog');
  if (!eventLog) return;
  
  if (events.length === 0) {
    eventLog.innerHTML = '<div class="text-center text-muted">No events logged yet</div>';
    return;
  }
  
  eventLog.innerHTML = events.map(event => `
    <div class="event-item ${event.type}">
      <div class="event-time">${new Date(event.timestamp).toLocaleString()}</div>
      <div class="event-message">${event.message}</div>
      ${event.data ? `<pre style="margin-top: 0.5rem; font-size: 0.75rem; opacity: 0.7;">${JSON.stringify(event.data, null, 2)}</pre>` : ''}
    </div>
  `).join('');
}

function clearEvents() {
  events.length = 0;
  updateEventLog();
  addEvent('info', 'Event log cleared');
}

function exportEvents() {
  const dataStr = JSON.stringify(events, null, 2);
  const blob = new Blob([dataStr], { type: 'application/json' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = `events_${Date.now()}.json`;
  a.click();
  URL.revokeObjectURL(url);
  addEvent('success', 'Events exported successfully');
}

function getAllEvents() {
  return [...events];
}
