/* Ripple Tank Firmware V1.x.x




Version 1.0.1
- Updates to water pump timing


*/


#include "globals.h"





void setup() {
  Serial.begin(115200);
  // Davids PWM Mod
  delay(2000);

  pwm.setClockDivider(1, false);  // Input clock is divided by 1 and 48MHz is sent to Generic Clock, Turbo is off
  pwm.timer(0, 1, 1262, true);    // Timer 1 is set to Generic Clock divided by 1, resolution is 960000, left-aligned aka single-slope PWM

  // Setup IO PIns
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(led_ctrl_pin, OUTPUT);
  // pinMode(wave_pin, OUTPUT);    // DO NOT SET THIS PIN TO OUTPUT IT BREAKS THE WAVE GENERATOR!
  // pwm.analogWrite(led_ctrl_pin, 0);  // need to do this first incase it hangs waiting for serial
  set_brightness(LIGHT_OFF);
  digitalWrite(LED_1, true);

  // Set Up SPI for digital pot volume control
  pinMode(digiPotSelectPin, OUTPUT);
  SPI.begin();
  set_brightness(0);
  digitalWrite(LED_1, false);

  while (!Serial) {
    delay(1);  // give time for Serial object to start
  }

  print_info();  // print the program info -> enables identification of current loaded firmware

  // set up wavetable
  analogWriteResolution(10);
  calc_wave_baseDelay(tableSize);
  set_frequency(0.1);
  delay(10);
  set_frequency(frequency);





  // Set Volume (now SPI has started)
  setVolume(amplitudeDefault);




  if (bme.begin()) {
    //Serial.println("{\"info\":\"Environment sensor found\"}");
  }

  pump_setup();
  set_direction(true);
  setupTimerTC5(35);

  //setupDAC();
  // delay(100);
  // wavetable_clock_setup();
  // get settings and cal data from memory
  // setFrequency(frequency);


  // Start/Calibrate Sensors -> load scales from memory


  delay(1000);  // delay to allow system to settle
  digitalWrite(LED_2, false);
  set_brightness(1);  // trying this line everywhere but no luck. Works as soon as its called from a state though???
  led.begin(true);    // blink beacon to show ready for use
  led.setDefault(true);
  led.callBlink();
}





void loop() {




  // Json Messenger & State Machine
  jsonStateData_t nextState_data = jsonRX.jsonReadSerialLoop();


  if (nextState_data.cmd_received) {  // If command is receive
    led.callBlink(4, 20, 60);
    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState_data.stateEnum != STATE_NULL) {
      smState = nextState_data.stateEnum;
    }
  }
  sm_Run(nextState_data);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command




  if (waveState == WAVE_ACTIVE) {
    run_wavetable();
    if (millis() - wave_start_time_mS >= (WAVE_MAX_TIME_S * 1000)) {
      waveState = WAVE_STOPPING;
      Serial.println("wavetable- timed out");
    }
  } else if (waveState == WAVE_STOPPING) {
    analogWrite(wave_pin, 0);  // reset to position less likely to contain noise or cause pops (pops preferable to noise)
    waveState = WAVE_STOPPED;
  } else if (waveState == WAVE_START) {
    table_index = 0;  // restart wavetable from the begginning
    waveState = WAVE_ACTIVE;
  } else if (waveState == WAVE_PULSE) {
    pulse_wavetable();
    waveState == WAVE_STOPPED;
  } else {
    // do nothing
  }
  // WAVE_PULSE is handled internal to state but could move here




  // Do sampling Data at the specified rate
  // Moved this to info state, as timing is critical in this situation
  // if (sampleDelay.millisDelay(sampleDelay_mS)) {
  //  if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the

  //  }
  // }






// Periodic Measurements & limits go here -> they might use data sampled above to make decisions
#if LIMITS_ENABLED == true
  if (led_power > 0) {  // timeout for LED light
    if (millis() - led_on_time_mS >= (LED_MAX_TIME_S * 1000)) {
      led_power = 0;
      // pwm.analogWrite(led_ctrl_pin, led_power);
      set_brightness(0);
    }
  }

#endif

  if (pumpState == PUMP_EMPTYING) {
        // code here to run pump
    // send_pulse();   // this is now an interrupt
    if (millis() - pump_start_time_mS >= empty_time_S * 1000) {
      pumpState = PUMP_STOPPED;
      Serial.println("pump empty timeout");
      stop_pump();
      delayMicroseconds(4);
      // make sure stepper driver shut down properly
      disable_pump();
    }
  }

  if (pumpState == PUMP_REFILLING) {
    // code here to run pump
    // send_pulse();  // this is now an interrupt
    if (millis() - pump_start_time_mS >= refill_time_S * 1000) {
      pumpState = PUMP_STOPPED;
      Serial.println("pump refill timeout");
      stop_pump();
      delayMicroseconds(4);
      // make sure stepper driver shut down properly
      disable_pump();
    }
  }

  if (pumpState != PUMP_STOPPED || streaming_active) {
    sample_tank();
    sample_data();
  }







  // do streaming data at the specified rate



  if (samples_written >= num_samples_req) {  // REMOVED PRINT TIMER because the number of samples taken is already calculated to meet the printing time   // if (printDelay.millisDelay(print_delay_mS)) {
    sampleDelay.resetDelayTime_mS();         // makes sure that the sample loop is synced to the printing loop //moved to try and improve timings (doing this first so next sample is sooner)
    if (streaming_active || snapshop_active) {
      //print the sampled data
      update_json(samples_written, smState);
    }
    samples_written = 0;
  }


  // Time Out Tools and utility loop functions
  led.performBlink();
 
}



// Interrupt Functions if Required
// Interrupt on A changing state
void isr_A() {
  // encoder._isr_A();  // example of calling ISR from library
}

// Interrupt on B changing state
void isr_B() {
  //encoder._isr_B();
}
