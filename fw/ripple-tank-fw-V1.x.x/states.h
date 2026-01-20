
/* States.h

Here use the templates to fully define the contents of each state. 

Good states should set variables and do things quickly, then exit so the state machine can continue to run.

Blocking states are acceptable if you understand why they will be blocking and account for this within the implementation.

e.g. In these states, the pulse state is blocking while it sends one wave out. This is okay because this event happens very quickly and then exits immediatly
 

*/


void sm_state_null(jsonStateData_t &stateData) {
  Serial.println(F("ERROR: ->NULL STATE ENTERED!"));
  stateData.uInt++;
  smState = STATE_WAIT;
}

// Init state, only called at startup
void sm_state_init(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println("state: INIT");
#endif
    stateData.uInt++;
  }

  smState = STATE_WAIT;
}




// State Wait is the default state for this program
void sm_state_wait(jsonStateData_t &stateData) {
  if (lastState != smState) {
    stateData.uInt++;
    // If first iteration print state machine status
#if DEBUG_STATES == true
    Serial.println(F("state: WAIT"));
#endif
#if COMMAND_HINTS == true  // print suggested commands
    Serial.println(F("\nEnter cmd in format:"));
    jsonRX.print_cmds();
#endif
    lastState = smState;
  }
}


void sm_state_light(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: LIGHT"));
#endif

    if (stateData.uInt >= 0 && stateData.uInt <= 100) {
      led_power = map(stateData.uInt, 0, 100, 0, 255);
      Serial.print("{\"brightness\":\"");
      Serial.print(led_power);
      Serial.println("\"}");
      analogWrite(LED_CTRL, led_power);
      led_on_time_mS = millis();
    } else {
      Serial.println("Requested lighting value OUT OF BOUNDS");
    }
    lastState = smState;
  }
  smState = STATE_WAIT;
}


void sm_state_hz(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: HZ"));
#endif
    lastState = smState;
    if (stateData.floatData > 0 && stateData.floatData < 45) {
      frequency = stateData.floatData;
      set_frequency(frequency);
      wave_start_time_mS = millis();
      // setFrequency(frequency);   // interrupt method (doesnt work)
    } else {
      Serial.println("Frequency DATA OUT OF BOUNDS");
    }
  }
  smState = STATE_WAIT;
}



void sm_state_amp(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: AMP"));
#endif
    lastState = smState;
    if (stateData.uInt >= 0 && stateData.uInt <= 100) {
      amplitude = map(stateData.uInt, 0, 100, 0, 255);
      Serial.print("{\"amplitude\":\"");
      Serial.print(amplitude);
      Serial.println("\"}");
      setVolume(amplitude);
      wave_start_time_mS = millis();
    } else {
      Serial.println("Requested amplitude value OUT OF BOUNDS");
    }
  }
  smState = STATE_WAIT;
}

void sm_state_start(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: START"));
#endif
    lastState = smState;
    wavetable_active = true;
    wave_start_time_mS = millis();
  }
  smState = STATE_WAIT;
}

void sm_state_stop(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOP"));
#endif
    lastState = smState;
    wavetable_active = false;
  }
  smState = STATE_WAIT;
}

void sm_state_pulse(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: PULSE"));
#endif
    lastState = smState;
    wavetable_active = false;
    pulse_active = true;
    table_index = 0;
  }
  uint16_t tableVal = 0;
  while (pulse_active) {
    if (waveTableDelay.microsDelay(waveDelayTime_uS)) {
      tableVal = pgm_read_word(&sineTable[table_index]);
      analogWrite(wave_pin, tableVal);
      // Serial.println(tableVal);
      table_index++;
      if (table_index >= (tableSize - 35)) pulse_active = false;  // not using the whole table to avoid the zero crossing overshoot that is then corrected by the DC blocking caps, leading to a pop
    }
  }
  table_index = 0;
  smState = STATE_WAIT;
}



void sm_state_pumpout(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: PUMPOUT"));
#endif
    lastState = smState;
    pumpState = PUMP_EMPTYING;
    pump_start_time_mS = millis();
    set_direction(false);
    enable_pump();
  }
  smState = STATE_WAIT;
}

void sm_state_pumpin(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: PUMPIN"));
#endif
    lastState = smState;
    pumpState = PUMP_REFILLING;
    pump_start_time_mS = millis();
    set_direction(true);
    enable_pump();
  }
  smState = STATE_WAIT;
}






// Change the samplerate of streamed or snapshotted data (Init at 10 Hz)
void sm_state_samplerate(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: SAMPLERATE"));
#endif
    lastState = smState;
  }
  if (stateData.uInt < 1 || stateData.uInt > 200) {
    // errors.set_error(false, -10, "Out of Bounds Sample Rate Commanded", errors.WARNING, "state-samplerate");
    //  errors.print_json_status();
  } else if (stateData.uInt < print_rate_Hz) {
    //  errors.set_error(false, -10, "Cannot set sample rate less than print rate", errors.WARNING, "state-samplerate");
    //  errors.print_json_status();
  } else {
    //   sampleRate_Hz = stateData.uInt;
    // sampleDelay_mS = 1000 / sampleRate_Hz;
    //   sampleDelay_mS = 1E3 / sampleRate_Hz - SAMPLE_DELAY_OFFSET;
    //   num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle
  }
  smState = STATE_WAIT;
}



// Change the samplerate of streamed or snapshotted data (Init at 10 Hz)
void sm_state_printrate(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: PRINT RATE"));
#endif
    lastState = smState;
  }
  if (stateData.uInt < 1 || stateData.uInt > 50) {
    //   errors.set_error(false, -10, "Out of Bounds Print Rate Commanded", errors.WARNING, "state-printrate");
    //   errors.print_json_status();
  } else if (stateData.uInt > sampleRate_Hz) {
    //   errors.set_error(false, -10, "Cannot set print rate greater than sample rate", errors.WARNING, "state-printrate");
    //   errors.print_json_status();
  } else {
    //  print_rate_Hz = stateData.uInt;
    //  print_delay_mS = 1000 / print_rate_Hz;
    //  num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle
  }
  smState = STATE_WAIT;
}





// Start streaming data
void sm_state_start_stream(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: STARTSTREAM"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  streaming_active = true;
  samples_written = 0;  // start sampling again, wondering if this is the cause of the issues
  //  snapshot_timer_mS = jsonStateData_t.signedInt;
  smState = STATE_WAIT;
}



// Stop streaming data
void sm_state_stop_stream(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOPSTREAM"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  streaming_active = false;
  // snapshot_timer_mS = 0;
  smState = STATE_WAIT;
}


// NEWER FUNCTIONS from here -------------------------------->>>>

// Trigger a snapshot of data to be taken over X amount of time
void sm_state_set_secret(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET SECRET"));
#endif
    lastState = smState;
    if (stateData.msg[0] == '\0') {  //empty string
      Serial.println(F("{\"WARNING\":\"Secret not Set\"}"));
      smState = STATE_WAIT;
      return;
    } else {
    }
  }
  smState = STATE_WAIT;
}

void sm_state_reset(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: RESET"));
#endif
    lastState = smState;
  }
  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_info(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    stateData.uInt++;
    lastState = smState;
    update_json(1, smState);
  }

  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_help(jsonStateData_t &stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  print_cmds();
  smState = STATE_WAIT;
}
