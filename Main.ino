// Autonomous Water Treatment Prototype Skeleton
// Focus: State machine architecture with TODOs for detailed implementation.

// -------------------------
// Enumerations & Globals
// -------------------------

enum State {
  STATE_IDLE,
  STATE_INIT_TURBIDITY,
  STATE_DISPENSE_ALUM,
  STATE_PUMP_DIRTY,
  STATE_MIX_FAST,
  STATE_MIX_SLOW,
  STATE_SETTLE,
  STATE_PRESS,
  STATE_PUMP_CLEAN,
  STATE_FINAL_TURBIDITY,
  STATE_DONE,
  STATE_ESTOP
};

State currentState = STATE_IDLE;
unsigned long stateStartTime = 0;

// Forward declarations for state machine helpers
void enterState(State nextState);

void updateIdle();
void updateInitTurbidity();
void updateDispenseAlum();
void updatePumpDirty();
void updateFastMix();
void updateSlowMix();
void updateSettle();
void updatePress();
void updatePumpClean();
void updateFinalTurbidity();
void updateDone();
void updateEstop();

void startIdle();
void startInitTurbidity();
void startDispenseAlum();
void startPumpDirty();
void startFastMix();
void startSlowMix();
void startSettle();
void startPress();
void startPumpClean();
void startFinalTurbidity();
void startDone();
void startEstop();

// -------------------------
// Hardware Configuration
// -------------------------

// TODO: Assign actual pin numbers according to wiring.
const int PIN_START_BUTTON = 2;
const int PIN_ESTOP_BUTTON = 3;
const int PIN_TURBIDITY_SENSOR = A0;

const int PIN_PUMP_DIRTY = 4;
const int PIN_PUMP_CLEAN = 5;
const int PIN_MIXER_SPEED = 6;   // PWM pin for speed control
const int PIN_MIXER_DIR = 7;      // Direction or enable pin depending on motor driver
const int PIN_PRESS_MOTOR = 8;
const int PIN_PRESS_DIRECTION = 9;

// -------------------------
// Utility Helpers
// -------------------------

bool readStartButton();
bool readEstopButton();
float readTurbidityVoltage();
float voltageToNTU(float voltage);
void stopAllMotors();

void pumpDirtyOn();
void pumpDirtyOff();

void pumpCleanOn();
void pumpCleanOff();

void mixerFast();
void mixerSlow();
void mixerOff();

void pressDown();
void pressUp();
void pressOff();

bool hasElapsed(unsigned long duration);

// -------------------------
// Arduino Setup & Loop
// -------------------------

void setup() {
  // Configure pin modes
  pinMode(PIN_START_BUTTON, INPUT_PULLUP);  // TODO: Adjust if using different wiring
  pinMode(PIN_ESTOP_BUTTON, INPUT_PULLUP);  // TODO: Adjust if using different wiring
  pinMode(PIN_TURBIDITY_SENSOR, INPUT);

  pinMode(PIN_PUMP_DIRTY, OUTPUT);
  pinMode(PIN_PUMP_CLEAN, OUTPUT);
  pinMode(PIN_MIXER_SPEED, OUTPUT);
  pinMode(PIN_MIXER_DIR, OUTPUT);
  pinMode(PIN_PRESS_MOTOR, OUTPUT);
  pinMode(PIN_PRESS_DIRECTION, OUTPUT);

  stopAllMotors();

  Serial.begin(9600);
  while (!Serial) {
    // Wait for Serial connection if needed (optional for boards other than UNO)
  }

  Serial.println("System Boot: Water Treatment Prototype");
  // TODO: Optional - take baseline turbidity reading for calibration and print.

  enterState(STATE_IDLE);
}

void loop() {
  // Read critical inputs first.
  const bool estopPressed = readEstopButton();
  const bool startPressed = readStartButton();

  if (estopPressed) {
    if (currentState != STATE_ESTOP) {
      enterState(STATE_ESTOP);
    }
    updateEstop();
    return;  // Ensure we do nothing else in this loop iteration.
  }

  // Handle start button when idle.
  if (currentState == STATE_IDLE && startPressed) {
    // TODO: Optionally debounce or require edge detection.
    enterState(STATE_INIT_TURBIDITY);
  }

  switch (currentState) {
    case STATE_IDLE:
      updateIdle();
      break;
    case STATE_INIT_TURBIDITY:
      updateInitTurbidity();
      break;
    case STATE_DISPENSE_ALUM:
      updateDispenseAlum();
      break;
    case STATE_PUMP_DIRTY:
      updatePumpDirty();
      break;
    case STATE_MIX_FAST:
      updateFastMix();
      break;
    case STATE_MIX_SLOW:
      updateSlowMix();
      break;
    case STATE_SETTLE:
      updateSettle();
      break;
    case STATE_PRESS:
      updatePress();
      break;
    case STATE_PUMP_CLEAN:
      updatePumpClean();
      break;
    case STATE_FINAL_TURBIDITY:
      updateFinalTurbidity();
      break;
    case STATE_DONE:
      updateDone();
      break;
    case STATE_ESTOP:
      // Already handled above, but keep default safety.
      updateEstop();
      break;
  }
}

// -------------------------
// State Machine Core
// -------------------------

void enterState(State nextState) {
  currentState = nextState;
  stateStartTime = millis();

  switch (nextState) {
    case STATE_IDLE:
      startIdle();
      break;
    case STATE_INIT_TURBIDITY:
      startInitTurbidity();
      break;
    case STATE_DISPENSE_ALUM:
      startDispenseAlum();
      break;
    case STATE_PUMP_DIRTY:
      startPumpDirty();
      break;
    case STATE_MIX_FAST:
      startFastMix();
      break;
    case STATE_MIX_SLOW:
      startSlowMix();
      break;
    case STATE_SETTLE:
      startSettle();
      break;
    case STATE_PRESS:
      startPress();
      break;
    case STATE_PUMP_CLEAN:
      startPumpClean();
      break;
    case STATE_FINAL_TURBIDITY:
      startFinalTurbidity();
      break;
    case STATE_DONE:
      startDone();
      break;
    case STATE_ESTOP:
      startEstop();
      break;
  }
}

bool hasElapsed(unsigned long duration) {
  return millis() - stateStartTime >= duration;
}

// -------------------------
// Stage Start & Update Helpers
// -------------------------

void startIdle() {
  stateStartTime = millis();
  stopAllMotors();
  Serial.println("State: Idle - Waiting for Start");
  // TODO: Add any idle state initialization (e.g., indicator LEDs).
}

void updateIdle() {
  // TODO: Monitor system while idle (e.g., display status, perform background diagnostics).
}

void startInitTurbidity() {
  stateStartTime = millis();
  Serial.println("Stage 1: Initial Turbidity Logging");
  // TODO: Initialize data logging buffers or counters for turbidity sampling.
}

void updateInitTurbidity() {
  // TODO: Continuously sample turbidity sensor for ~1 minute and log readings.
  // Example placeholder:
  // float voltage = readTurbidityVoltage();
  // float ntu = voltageToNTU(voltage);
  // Serial.print("Stage 1 NTU: ");
  // Serial.println(ntu);
  // TODO: Decide when the logging period is complete using hasElapsed().
  // if (hasElapsed(60000UL)) {
  //   enterState(STATE_DISPENSE_ALUM);
  // }
}

void startDispenseAlum() {
  stateStartTime = millis();
  Serial.println("Stage 2: Dispense Alum Slurry");
  // TODO: Activate the actuator (e.g., valve or pump) responsible for alum dosing.
}

void updateDispenseAlum() {
  // TODO: Control alum dispensing duration/quantity.
  // TODO: When dosing complete, transition to dirty water pumping stage.
  // enterState(STATE_PUMP_DIRTY);
}

void startPumpDirty() {
  stateStartTime = millis();
  Serial.println("Stage 3: Pump Dirty Water");
  pumpDirtyOn();
  // TODO: Initialize counters/flow measurement if available to track volume.
}

void updatePumpDirty() {
  // TODO: Monitor volume or time to determine when coagulation tank is filled.
  // TODO: Once satisfied, stop pump and transition.
  // pumpDirtyOff();
  // enterState(STATE_MIX_FAST);
}

void startFastMix() {
  stateStartTime = millis();
  Serial.println("Stage 4: Fast Mixing");
  mixerFast();
  // TODO: Prepare any fast-mix specific logging if required.
}

void updateFastMix() {
  // TODO: Maintain fast mixing for required duration (>= 10 s) using hasElapsed().
  // if (hasElapsed(10000UL)) {
  //   mixerSlow(); // or call mixerOff() before switching states.
  //   enterState(STATE_MIX_SLOW);
  // }
}

void startSlowMix() {
  stateStartTime = millis();
  Serial.println("Stage 5: Slow Mixing");
  mixerSlow();
  // TODO: Adjust PWM or control for slow speed.
}

void updateSlowMix() {
  // TODO: Maintain slow mixing for required duration (>= 10 s) using hasElapsed().
  // if (hasElapsed(10000UL)) {
  //   mixerOff();
  //   enterState(STATE_SETTLE);
  // }
}

void startSettle() {
  stateStartTime = millis();
  Serial.println("Stage 6: Settling Pause");
  mixerOff();
  // TODO: Decide if settling is required; maybe skip directly if not needed.
}

void updateSettle() {
  // TODO: Wait for optional settling period (configurable) before pressing.
  // Example:
  // if (hasElapsed(configuredSettleDuration)) {
  //   enterState(STATE_PRESS);
  // }
}

void startPress() {
  stateStartTime = millis();
  Serial.println("Stage 7: Press Activation");
  pressDown();
  // TODO: Initialize sensors/limit switches for press position monitoring.
}

void updatePress() {
  // TODO: Monitor press operation (position, pressure) and determine when filtration complete.
  // TODO: Use stopAllMotors() or pressOff() as appropriate when cycle done.
  // enterState(STATE_PUMP_CLEAN);
}

void startPumpClean() {
  stateStartTime = millis();
  Serial.println("Stage 8: Pump Clean Water");
  pumpCleanOn();
  // TODO: Initialize counters/flow measurement to achieve >= 100 mL.
}

void updatePumpClean() {
  // TODO: Monitor clean water volume or run time to ensure >= 100 mL transferred.
  // TODO: When complete, stop pump and transition to final turbidity measurement.
  // pumpCleanOff();
  // enterState(STATE_FINAL_TURBIDITY);
}

void startFinalTurbidity() {
  stateStartTime = millis();
  Serial.println("Stage 9: Final Turbidity Logging");
  // TODO: Prepare for final turbidity sampling.
}

void updateFinalTurbidity() {
  // TODO: Sample turbidity sensor, convert to NTU, and log results similar to Stage 1.
  // TODO: After logging, transition to STATE_DONE.
  // enterState(STATE_DONE);
}

void startDone() {
  stateStartTime = millis();
  Serial.println("Stage 10: Cycle Complete - System Idle");
  stopAllMotors();
  // TODO: Signal completion (e.g., LED or buzzer) and wait for manual reset.
}

void updateDone() {
  // TODO: Stay in DONE state until manual reset condition is met (e.g., power cycle or button).
  // Potentially allow start button to re-arm after manual confirmation if desired.
}

void startEstop() {
  stateStartTime = millis();
  stopAllMotors();
  Serial.println("EMERGENCY STOP ACTIVATED - All actuators halted");
  // TODO: Engage additional safety interlocks if required.
}

void updateEstop() {
  // TODO: Maintain safe state; optionally monitor for reset condition without resetting Arduino.
  // System should require manual intervention to leave this state.
}

// -------------------------
// Hardware Abstraction Layer
// -------------------------

bool readStartButton() {
  // TODO: Implement proper debouncing/edge detection as required.
  return digitalRead(PIN_START_BUTTON) == LOW;  // Assuming active-low button
}

bool readEstopButton() {
  // TODO: Confirm wiring; ensure this reflects actual E-stop logic (prefer normally-closed).
  return digitalRead(PIN_ESTOP_BUTTON) == LOW;  // Assuming active-low E-stop
}

float readTurbidityVoltage() {
  // TODO: Read analog sensor and convert to voltage based on reference (typically 5V or 3.3V).
  // int raw = analogRead(PIN_TURBIDITY_SENSOR);
  // float voltage = (raw / 1023.0f) * 5.0f;
  // return voltage;
  return 0.0f;  // Placeholder
}

float voltageToNTU(float voltage) {
  return -1120.4f * voltage * voltage + 5742.3f * voltage - 4352.9f;
}

void stopAllMotors() {
  pumpDirtyOff();
  pumpCleanOff();
  mixerOff();
  pressOff();
  // TODO: Add additional actuators if integrated (e.g., valves, alarms).
}

void pumpDirtyOn() {
  // TODO: Implement motor shield/relay activation for dirty water pump.
  digitalWrite(PIN_PUMP_DIRTY, HIGH);
}

void pumpDirtyOff() {
  // TODO: Ensure pump is fully de-energized (consider motor shield direction pins).
  digitalWrite(PIN_PUMP_DIRTY, LOW);
}

void pumpCleanOn() {
  // TODO: Implement control for clean water pump.
  digitalWrite(PIN_PUMP_CLEAN, HIGH);
}

void pumpCleanOff() {
  // TODO: Ensure clean water pump is fully off.
  digitalWrite(PIN_PUMP_CLEAN, LOW);
}

void mixerFast() {
  // TODO: Configure motor driver for fast mixing (set direction, high PWM duty cycle).
  digitalWrite(PIN_MIXER_DIR, HIGH);  // Placeholder direction
  analogWrite(PIN_MIXER_SPEED, 255);  // Placeholder speed
}

void mixerSlow() {
  // TODO: Configure motor driver for slow mixing (set direction, lower PWM duty cycle).
  digitalWrite(PIN_MIXER_DIR, HIGH);  // Placeholder direction
  analogWrite(PIN_MIXER_SPEED, 128);  // Placeholder speed
}

void mixerOff() {
  // TODO: Fully stop mixer (zero PWM, disable driver if needed).
  analogWrite(PIN_MIXER_SPEED, 0);
}

void pressDown() {
  // TODO: Run press motor in downward direction (set direction, enable motor).
  digitalWrite(PIN_PRESS_DIRECTION, HIGH);
  digitalWrite(PIN_PRESS_MOTOR, HIGH);
}

void pressUp() {
  // TODO: Run press motor in upward direction if needed for retract sequence.
  digitalWrite(PIN_PRESS_DIRECTION, LOW);
  digitalWrite(PIN_PRESS_MOTOR, HIGH);
}

void pressOff() {
  // TODO: Stop press motor completely (disable outputs/brakes).
  digitalWrite(PIN_PRESS_MOTOR, LOW);
}

