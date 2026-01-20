/* JSON State Machine


VERSION 3.0.0
- Now at close to minimum memory requirements for this topology
(compiled for Arduino Nano)
Sketch uses 21134 bytes (68%) of program storage space. Maximum is 30720 bytes.
Global variables use 932 bytes (45%) of dynamic memory, leaving 1116 bytes for local variables. Maximum is 2048 bytes.
- changed all state machine functions to pass by reference so new copies of data are not created in both state machine function and state function when called
(this shouldnt change the global overhead much, but it does stop additional data being created while within each state)
Sketch uses 21058 bytes (68%) of program storage space. Maximum is 30720 bytes.
Global variables use 904 bytes (44%) of dynamic memory, leaving 1144 bytes for local variables. Maximum is 2048 bytes.
- did change it slightly though, I assume because the function pointer structure doesnt need to make memory space for that data structure

Version 3.0.1
-Removed secrets due to compatability with IoT 33 SAMD platforms
Sketch uses 16190 bytes (52%) of program storage space. Maximum is 30720 bytes.
Global variables use 813 bytes (39%) of dynamic memory, leaving 1235 bytes for local variables. Maximum is 2048 bytes


*/


#include "globals.h"





void setup() {
  Serial.begin(115200);


  // Setup IO PIns
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_CTRL, OUTPUT);
  analogWrite(LED_CTRL, 0);  // need to do this first incase it hangs waiting for serial

  // Set Up SPI for digital pot volume control
  pinMode(digiPotSelectPin, OUTPUT);
  SPI.begin();

  // set up wavetable
  calc_wave_baseDelay(tableSize);
  analogWriteResolution(10);
  set_frequency(0.1);
  delay(10);
  set_frequency(frequency);
  setVolume(amplitude);

  while (!Serial) {
    delay(1);  // give time for Serial object to start
  }
  //delay(2000); // dont wait for serial as powered PCB will turn on LED untill serial connects -> just changed the order of operation now

  print_info();  // print the program info -> enables identification of current loaded firmware

  if (bme.begin()) {
    Serial.println("{\"info\":\"Environment sensor found\"}");
  }


  // get settings and cal data from memory



  // Start/Calibrate Sensors -> load scales from memory


  delay(1000);  // delay to allow system to settle

  // Tare/zero sensors
}





void loop() {


  // Json Messenger & State Machine
  jsonStateData_t nextState_data = jsonRX.jsonReadSerialLoop();


  if (nextState_data.cmd_received) {  // If command is receive
    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState_data.stateEnum != STATE_NULL) {
      smState = nextState_data.stateEnum;
    }
  }
  sm_Run(nextState_data);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command










  // Do sampling Data at the specified rate
  if (sampleDelay.millisDelay(sampleDelay_mS)) {
    if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the
      timestamp_array[samples_written] = millis();
      sensors_event_t temp_event, pressure_event, humidity_event;
      bme_temp->getEvent(&temp_event);
      bme_pressure->getEvent(&pressure_event);
      bme_humidity->getEvent(&humidity_event);
      ambient_temp[samples_written] = temp_event.temperature;
      ambient_press[samples_written] = pressure_event.pressure;
      ambient_humid[samples_written] = humidity_event.relative_humidity;
      //arbitaryData++;  // just a placeholder to generate moving data
      samples_written++;
      //Serial.println(humidity_event.relative_humidity);
    }
  }



  if (wavetable_active) {
    run_wavetable();
    if (millis() - wave_start_time_mS >= (WAVE_MAX_TIME_S * 1000)) {
      wavetable_active = false;
      Serial.println("wavetable- timed out");
    }
  } else {
    analogWrite(wave_pin, 0);
  }



// Periodic Measurements & limits go here -> they might use data sampled above to make decisions
#if LIMITS_ENABLED == true
  if (led_power > 0) {  // timeout for LED light
    if (millis() - led_on_time_mS >= (LED_MAX_TIME_S * 1000)) {
      led_power = 0;
      analogWrite(LED_CTRL, led_power);
    }
  }

#endif











  // do streaming data at the specified rate

  if (samples_written >= num_samples_req) {  // REMOVED PRINT TIMER because the number of samples taken is already calculated to meet the printing time   // if (printDelay.millisDelay(print_delay_mS)) {
    sampleDelay.resetDelayTime_mS();         // makes sure that the sample loop is synced to the printing loop //moved to try and improve timings (doing this first so next sample is sooner)
    if (streaming_active || snapshop_active) {
      //print the sampled data
      update_json(samples_written);
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
