

autoDelay pumpDelay;
const int PULSE_HIGH_mS = 100;
const int PULSE_LOW_mS = 200;
int pulse_state = false;

// ADD STOP PUMP STATE

void pump_setup() {
  pinMode(pump_step_pin, OUTPUT);
  pinMode(pump_dir_pin, OUTPUT);
  pinMode(pump_EN_pin, OUTPUT);
  pinMode(pump_sleep_pin, OUTPUT);
  pinMode(pump_reset_pin, OUTPUT);
}


void enable_pump() {
  digitalWrite(pump_EN_pin, 0);
  digitalWrite(pump_sleep_pin, 1);
  digitalWrite(pump_reset_pin, 1);
}


void disable_pump() {
  digitalWrite(pump_step_pin, 0);
  digitalWrite(pump_dir_pin, 0);
  digitalWrite(pump_EN_pin, 0);
  digitalWrite(pump_sleep_pin, 0);
  digitalWrite(pump_reset_pin, 0);
}

void set_direction(bool direction = false) {
  digitalWrite(pump_dir_pin, direction);
}

void send_pulse() {
  if (!pulse_state) {
    if (pumpDelay.millisDelay(PULSE_LOW_mS)) {
      pulse_state = 1;
      digitalWrite(pump_step_pin, pulse_state);
    }
  } else {
    if (pumpDelay.millisDelay(PULSE_HIGH_mS)) {
      pulse_state = 0;
      digitalWrite(pump_step_pin, pulse_state);
    }
  }
}