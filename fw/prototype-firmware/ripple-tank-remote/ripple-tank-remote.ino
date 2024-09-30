/*
Firmware for the ripple-tank remote laboratory

Prototype firmware by Imogen Heard.
This version by David Reid
State machine boilerplate from https://www.edn.com/electronics-blogs/embedded-basics/4406821/Function-pointers---Part-3--State-machines
*/

#include "ArduinoJson-v6.9.1.h"
#include <TimerInterrupt.h>
#include <TimerInterrupt.hpp>
#include <ISR_Timer.h>
#include <ISR_Timer.hpp>

#define COMMAND_SIZE 64
StaticJsonDocument<COMMAND_SIZE> doc;
char command[COMMAND_SIZE];

//PIN ASSIGNMENT
#define SPEAKER_PIN 3
#define LED_PWM_PIN 9
#define BRIGHTNESS_PIN A0
#define STROBE_PIN A2
#define FREQUENCY_PIN A4

unsigned long PRINT_INTERVAL = 1000000;
int BRIGHTNESS = 50;

unsigned long previous_ripple_time = 0;
unsigned long previous_report_time = 0;
bool wave_state = false;

unsigned long frequency = 0;
unsigned long period_uS = 0;
unsigned long brightness = 0;

bool SET_ABS_FREQ = true;
unsigned long new_frequency = 0;          //Absolute value in Hz.
unsigned long new_frequency_percent = 0;  // between MIN_FREQUENCY_PERCENT and MAX_FREQUENCY_PERCENT
unsigned long MAX_FREQUENCY = 200;    // Hz
unsigned long MIN_FREQUENCY_PERCENT = 0;    // % 
unsigned long MAX_FREQUENCY_PERCENT = 100;    // %


/**
 * Defines the valid states for the state machine
 * 
 */
typedef enum
{
  STATE_STOPPED,
  STATE_START,
  STATE_OSCILLATE,
  STATE_UPDATE_FREQUENCY

} StateType;

//state Machine function prototypes
void Sm_State_Stopped(void);
void Sm_State_Start(void);
void Sm_State_Oscillate(void);
void Sm_State_Update_Frequency(void);

/**
 * Type definition used to define the state
 */
 
typedef struct
{
  StateType State; /**< Defines the command */
  void (*func)(void); /**< Defines the function to run */
} StateMachineType;
 
/**
 * A table that defines the valid states of the state machine and
 * the function that should be executed for each state
 */
StateMachineType StateMachine[] =
{
  {STATE_STOPPED, Sm_State_Stopped},
  {STATE_START, Sm_State_Start},
  {STATE_OSCILLATE, Sm_State_Oscillate},
  {STATE_UPDATE_FREQUENCY, Sm_State_Update_Frequency}
  
};
 
int NUM_STATES = 4;

/**
 * Stores the current state of the state machine
 */
 
StateType SmState = STATE_STOPPED;
StateType SmPreviousState = STATE_STOPPED;


void report(void);

void report(void)
{
  Serial.print("{\"SmState\":");
  Serial.print(SmState);
  Serial.print(",\"time\":");
  Serial.print(millis());  
  Serial.print(",\"brightness\":");
  Serial.print(brightness);
  Serial.print(",\"frequency\":");
  Serial.print(frequency);
  Serial.print(",\"amp\":");
  Serial.print(frequency);
  Serial.print(",\"period_uS\":");
  Serial.print(period_uS);
  Serial.println("}");
}

void Sm_State_Stopped(void){

  if(SmPreviousState != STATE_STOPPED){
    digitalWrite(SPEAKER_PIN, false);
  }
  
  SmState = STATE_STOPPED;
  SmPreviousState = STATE_STOPPED;
}

void Sm_State_Start(void){
  
  report();

  SmState = STATE_OSCILLATE;
  SmPreviousState = STATE_START;
}

// TODO
// void Sm_State_Update_Brightness(void){
  
//   SmState = SmPreviousState;
//   SmPreviousState = STATE_UPDATE_BRIGHTNESS;
// }

void Sm_State_Update_Frequency(void){
  
  if(SET_ABS_FREQ){

    frequency = new_frequency;

  } else{
    //a new frequency variable has been set as a percentage, 
    // but equate that with the driving frequency in Hz
    frequency = new_frequency_percent*MAX_FREQUENCY / 100;
  }
  

  report();

  //update state
  SmState = SmPreviousState;
  SmPreviousState = STATE_UPDATE_FREQUENCY;
}

// TODO
// void Sm_State_Pulse(void){
  

// }

void Sm_State_Oscillate(void){

  unsigned long current_time = micros();
  
  period_uS = 1000000 / frequency;

  if (frequency > 0) {
    if (current_time - previous_ripple_time >= period_uS / 2) {
      wave_state = !wave_state;
      previous_ripple_time = current_time;
      digitalWrite(SPEAKER_PIN, wave_state);
    }
  } else {
      digitalWrite(SPEAKER_PIN, false);
  }


  SmState = STATE_OSCILLATE;
  SmPreviousState = STATE_OSCILLATE;
}



void setup() {
  
  TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  
  pinMode(LED_PWM_PIN, OUTPUT);
  brightness = BRIGHTNESS;
  analogWrite(LED_PWM_PIN, brightness);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, false);

  Serial.setTimeout(50);
  Serial.begin(57600);
  Serial.println("Ripple Tank - Prototype");
}


void loop(){
  // for temporary manual control
  brightness = map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 254);
  analogWrite(LED_PWM_PIN, brightness);
  //frequency = map(analogRead(FREQUENCY_PIN), 0, 1023, 0, 300);

  //Eventually just include:
  Sm_Run();
  
} 

/**
 *  Example commmands
{"set":"start"}
{"set":"stop"}
{"set":"pulse", "to": 2}
{"set":"frequency", "to":10}  // 0-100% of maximum
{"set":"brightness", "to":10}	 // 0 - 100%
 */

StateType readSerialJSON(StateType SmState){
  
  if (Serial.available() > 0) 
  {

    char start[] = "start";
    char stop[] = "stop";
    char report_interval[] = "interval";
    char freq[] = "frequency";
    
    Serial.readBytesUntil(10, command, COMMAND_SIZE);
                
    deserializeJson(doc, command);

    const char* set = doc["set"];
    
    if (strcmp(set,report_interval)==0)
    {
      int new_interval = doc["to"];
      if ((new_interval > 0) && (new_interval < 1000))
      {
        PRINT_INTERVAL = new_interval;
        Serial.println("{\"result\":\"ok\"}");
      }
      else
      {
        Serial.println("{\"error\":\"interval must be between 0 - 1000\"}");
      }
    }
    
    if (strcmp(set,stop)==0)
    {

      SmState = STATE_STOPPED;

    }
    
    if (strcmp(set,start)==0)
    {
    
      if (SmState == STATE_STOPPED)
      { 

        SmState = STATE_START;

      }
      else{
      
        Serial.println("{\"error\":\"In wrong state to start\"}");

      }
    }

    if (strcmp(set,freq)==0)
    {
      unsigned long new_freq = doc["to"];
      if(SET_ABS_FREQ){

        new_frequency = new_freq;
        SmState = STATE_UPDATE_FREQUENCY;

      } else{
        if(new_freq >= MIN_FREQUENCY_PERCENT && new_freq <= MAX_FREQUENCY_PERCENT){

          new_frequency_percent = new_freq;
          SmState = STATE_UPDATE_FREQUENCY;

        } else{
          Serial.println("{\"error\":\"frequency out of range\"}");
        }
      }
      
      

    }

  } //if bytes available
  return SmState;
}

void Sm_Run(void)
{
  if (SmState < NUM_STATES)
  {
    SmState = readSerialJSON(SmState);
    (*StateMachine[SmState].func)();
  }
  else{
    Serial.println("Exception in State Machine");
  }
  
}







