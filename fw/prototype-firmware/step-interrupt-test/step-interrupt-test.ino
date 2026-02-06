#if !(defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD51__) || defined(__SAMD51J20A__) \
      || defined(__SAMD51J19A__) || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) \
      || defined(__SAMD21E15A__) || defined(__SAMD21E16A__) || defined(__SAMD21E17A__) || defined(__SAMD21E18A__) \
      || defined(__SAMD21G15A__) || defined(__SAMD21G16A__) || defined(__SAMD21G17A__) || defined(__SAMD21G18A__) \
      || defined(__SAMD21J15A__) || defined(__SAMD21J16A__) || defined(__SAMD21J17A__) || defined(__SAMD21J18A__))
#error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

/////////////////////////////////////////////////////////////////

// These define's must be placed at the beginning before #include "SAMDTimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define USING_TIMER_TC3 true   // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4 false  // Not to use with Servo library
#define USING_TIMER_TC5 false
#define USING_TIMER_TCC false
#define USING_TIMER_TCC1 false
#define USING_TIMER_TCC2 false  // Don't use this, can crash on some boards

#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

// TC3, TC4, TC5 max permissible HW_TIMER_INTERVAL_MS is 1398.101 ms, larger will overflow, therefore not permitted
// Use TCC, TCC1, TCC2 for longer HW_TIMER_INTERVAL_MS
#define HW_TIMER_INTERVAL_MS 10

///////////////////////////////////////////////

#if (TIMER_INTERRUPT_USING_SAMD21)

#if USING_TIMER_TC3
#define SELECTED_TIMER TIMER_TC3
#elif USING_TIMER_TC4
#define SELECTED_TIMER TIMER_TC4
#elif USING_TIMER_TC5
#define SELECTED_TIMER TIMER_TC5
#elif USING_TIMER_TCC
#define SELECTED_TIMER TIMER_TCC
#elif USING_TIMER_TCC1
#define SELECTED_TIMER TIMER_TCC1
#elif USING_TIMER_TCC2
#define SELECTED_TIMER TIMER_TCC
#else
#error You have to select 1 Timer
#endif

#else

#if !(USING_TIMER_TC3)
#error You must select TC3 for SAMD51
#endif

#define SELECTED_TIMER TIMER_TC3

#endif

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

// Init SAMD_ISR_Timer
// Each SAMD_ISR_Timer can service 16 different ISR-based timers
SAMD_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_400uS 400L

void TimerHandler(void) {
  ISR_Timer.run();
}

// Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
//HardwareTimer *MyTim = new HardwareTimer(Instance);


#include <autoDelay.h>



// Pump
const int pump_step_pin = 6;
const int pump_dir_pin = 4;
const int pump_EN_pin = 2;
const int pump_fault_pin = A3;
const int pump_sleep_pin = A6;
const int pump_reset_pin = A7;



autoDelay pumpDelay;
const int PULSE_HIGH_uS = 50;
const int PULSE_LOW_uS = 50;
int pulse_state = false;

// ADD STOP PUMP STATE

/*
void sample_tank(){
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
*/


void pump_setup() {
  // pinMode(pump_step_pin, OUTPUT);   // equivilent below
  PORT->Group[0].DIRSET.reg = (1 << 20);
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

void send_pulse() {
  if (!pulse_state) {
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
}


// The following functions are suitable for placing within an interrupt + starting and stopping interrupts
//uint8_t current_pulse_state = 0;
// PORT->Group[0].OUTSET.reg = (1 << 20);  // write pin 6 high
// PORT->Group[0].OUTCLR.reg = (1 << 20);  // write pin 6 low

void update_pulse_out() {
  // current_pulse_state = !current_pulse_state;
  // digitalWrite(pump_step_pin, current_pulse_state);
  PORT->Group[0].OUTTGL.reg = (1 << 20);  // toggle the output pin directly at the register level
}



void start_pump() {
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  } else {
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));


    // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
    // You can use up to 16 timer for each ISR_Timer
    ISR_Timer.setInterval(TIMER_INTERVAL_400uS, update_pulse_out);
  }
}


void stop_pump() {
  ITimer.detachInterrupt();

  //ISR_TImer.detachInterrupt();

  PORT->Group[0].OUTCLR.reg = (1 << 20);  // write pin 6 low
}



void send_one_pulse() {
  digitalWrite(pump_step_pin, true);
  delayMicroseconds(400);
  digitalWrite(pump_step_pin, false);
  delayMicroseconds(400);
}




void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("StepperTesting");

  pump_setup();
  set_direction(true);
  enable_pump();
  delay(100);
  // send_one_pulse();
  //disable_pump();
  start_pump();
}

#define NUM_PULSES 200

volatile uint8_t pulses_sent = 0;

void loop() {
  if (millis() >= 20000) {
    stop_pump();
    delay(5000);
    disable_pump();
  }
  // for (int i = 0; i < NUM_PULSES; i++) {
  //    send_one_pulse();
  //  }

  // disable_pump();
}
