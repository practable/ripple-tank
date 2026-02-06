

autoDelay pumpDelay;
const int PULSE_HIGH_uS = 400;
const int PULSE_LOW_uS = 400;
int pulse_state = false;

// ADD STOP PUMP STATE


void sample_tank() {
  tank_level = analogRead(tank_level_sense_pin);
  if (tankStatus == TANK_FULL) {
    if (tank_level >= tank_threshold + tank_hysteresis) {
      tankStatus = TANK_EMPTY;
    }
  } else if (tankStatus == TANK_EMPTY) {
    if (tank_level <= tank_threshold - tank_hysteresis) {
      tankStatus = TANK_FULL;
    }
  }
}



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
  digitalWrite(pump_reset_pin, 1);
}

void set_direction(bool direction = false) {
  digitalWrite(pump_dir_pin, direction);
}


// functions here to try to use interrupts in order to get pulse timings fast enough
void start_pump(){

}

void stop_pump(){

}

void send_pulse() {
  digitalWrite(pump_step_pin, true);
  delayMicroseconds(400);
  digitalWrite(pump_step_pin, false);
  delayMicroseconds(400);
}

/*if (!pulse_state) {
    if (pumpDelay.microsDelay(PULSE_LOW_uS)) {
      pulse_state = 1;
      digitalWrite(pump_step_pin, pulse_state);
    }
  } else {
    if (pumpDelay.microsDelay(PULSE_HIGH_uS)) {
      pulse_state = 0;
      digitalWrite(pump_step_pin, pulse_state);
    }
  }
*/
