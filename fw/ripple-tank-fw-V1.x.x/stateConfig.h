/*  stateConfig.h

STEP 1:

- This header defines ALL active states for the state machine, via ENUM and Short text description saved on PROGMEM
 Imogen Heard 08/01/2026


*/

#pragma once



// 1. Define all the valid states for the state machine with an enum.
//    - This should include states triggered by user input, but may also contain states that are only accessable programatically
// - Must start with "NULL_STATE"
// - Must end with "NUM_STATES"
typedef enum {
  STATE_NULL,
  STATE_INIT,
  STATE_WAIT,
  STATE_LIGHT,
  STATE_HZ,
  STATE_AMP,
  STATE_START,
  STATE_STOP,
  STATE_PULSE,
  STATE_PUMPOUT,
  STATE_PUMPIN,
  STATE_STOP_PUMP,
  STATE_SAMPLERATE,
  STATE_PRINTRATE,
  STATE_STARTSTREAM,
  STATE_STOPSTREAM,
  STATE_SETSECRET,
  STATE_RESET,
  STATE_INFO,  // report settings, cal data and validity to user
  STATE_HELP,
  NUM_STATES  // Sentinal value lets us get the total number of states without manually counting. Do not forget this value, it is important for correct function
} stateDef_t;

//stateDef_t smState = STATE_INIT;
//stateDef_t lastState;


// 2. Then we define two variables to hold the current state enum, and the previous state enum. Holding both these values allows us to compare them as we enter a state,
//    to see if it is the first time we have entered the state.



// new way of defining these text strings to save precious RAM
// shortened strings to save memory
// longest string 12 chars
const char state_null[] PROGMEM = "NULL";
const char state_0[] PROGMEM = "INIT";
const char state_1[] PROGMEM = "WAIT";
const char state_2[] PROGMEM = "LIGHT";
const char state_3[] PROGMEM = "HZ";
const char state_4[] PROGMEM = "AMP";
const char state_5[] PROGMEM = "START";
const char state_6[] PROGMEM = "STOP";
const char state_7[] PROGMEM = "PULSE";
const char state_8[] PROGMEM = "PUMPOUT";
const char state_9[] PROGMEM = "PUMPIN";
const char state_10[] PROGMEM = "STPPUMP";
const char state_11[] PROGMEM = "SMPLRT";
const char state_12[] PROGMEM = "PRINTRT";
const char state_13[] PROGMEM = "STREAM";
const char state_14[] PROGMEM = "STPSTREAM";
const char state_15[] PROGMEM = "SETSECRET";
const char state_16[] PROGMEM = "RESET";
const char state_17[] PROGMEM = "INFO";
const char state_18[] PROGMEM = "HELP";


// using generic names means this shouldnt need to change much often

const char *const stateNames[] PROGMEM = {
  state_null,
  state_0, state_1, state_2, state_3, state_4, state_5, state_6, state_7,
  state_8, state_9, state_10, state_11, state_12, state_13, state_14, state_15,
  state_16, state_17, state_18
};

// 3. Define a State Names Array, this will allow us to print the enum above in human readable format
// if RAM available and PROGMEM running low use this older method
/*
const char stateNames[][20] PROGMEM = {
  "STATE_INIT",
  "STATE_WAIT",
  "STATE_SERVO",
  "STATE_HOME",
  "STATE_DO_CAL",
  "STATE_SAMPLERATE",
  "STATE_PRINTRATE",
  "STATE_STARTSTREAM",
  "STATE_STOPSTREAM",
  "STATE_PUTSECRET",
  "STATE_GETSECRET",
  "STATE_SAVECAL",
  "STATE_GETCAL",
  "STATE_INFO",
  "STATE_HELP"
};
*/
