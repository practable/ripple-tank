/*  globals.h
      Define all user set values, global constants, global vars, included libraries and global objects

      Imogen Heard
      21/03/2025
*/

#pragma once

// Imported Libraries
#include <ArduinoJson.h>  // installed version 6.21.5 [Arduino Library Manager]
#include <ledObject.h>
#include <autoDelay.h>
#include <SPI.h>
#include <MCP4151.h>

// Included Libraries

// Included Headers (Called from within any JSON-state-machine header or class)
#include "cmds.h"

// JSON-state-machine headers & libraries (Must be included in this order)
#include "stateConfig.h"  // must always be first as jsonConfig uses variables within
#include "jsonConfig.h"
#include "jsonMessenger.h"  // jsonMessenger also relies on variables desifined in jsonConfig
// stateMachine is defined at the end of this file, as it needs access to all globals

// Program Attributes
#define EXPERIMENT_NAME "ripple-tank"
#define FIRMWARE_VERSION "1.0.0"
#define DEVELOPER "Imogen-Wren"


// Hardware Definitions
// Pins & IOs
// Encoder
#define INTERRUPT_0_PIN 2
#define INTERRUPT_1_PIN 3

// LED / Indicators
#define LED_BUILTIN 13

// Hardware Consts
//spi
#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13
//led
#define LED_CTRL 5
// wave output
const int wave_pin = A0;
const int digiPotSelectPin = 10;

// Pump 
const int pump_step_pin = 6;
const int pump_dir_pin = 4;
const int pump_EN_pin = 2;
const int pump_fault_pin = A3;
const int pump_sleep_pin = A6;
const int pump_reset_pin = A7;

// Sensors
const int tank_level_sense_pin = A2;
const int tank_threshold = 300;
const int tank_hysteresis = 50;

// Limits & Timeouts
#define LIMITS_ENABLED true

// User Options & Program Config

// JSON Reporting
#define PRINT_JSON false
#define PRETTY_PRINT_JSON false
#define JSON_TX_BUFFER_SIZE 240


// Sample & Printing Rates
#define INIT_SAMPLE_RATE_Hz 1
#define INIT_PRINT_RATE_Hz 1


// Utility
#define EEPROM_ADDRESS 0xFF  // EEPROM location for calibration data if required

// Debugging
#define DEBUG_STATES false
#define DEBUG_STATE_MACHINE true



// Object Declaration
// Indicator LED
ledObject led(LED_BUILTIN);
// jsonMessenger Object to handle incoming Serial JSON commands
jsonMessenger jsonRX;  // create a jsonMessenger object to handle commands received over Serial connection
// Delay Objects
autoDelay sampleDelay;
autoDelay printDelay;






// LED datas
const uint32_t LED_MAX_TIME_S = 300;
uint8_t led_power = 0;
uint32_t led_on_time_mS = 0;

// Ripples Datas
const int tableSize = 240;  // Number of samples in the sine wave table
// constants for different sine tables
const int low_table_size = 240;  // up to 30 Hz
const int mid_table_size = 120;  // 30 - 60 Hz
const int high_table_size = 60;  // 60 to 120 Hz

/*
typedef uint8_t enum {
  LOW_HZ_TABLE,
  MID_HZ_TABLE,
  HIGH_HZ_TABLE
} activeTable;
*/

// Variables for sine wave control
//float frequency = 1.0;  // Frequency of the sine wave in Hz
// One complete cycle of wave = {tableSize} samples
//-> for 1Hz operation, 1,000,000 uS / tableSize = {wavedelaytime} 3906.25 uS of delay between each sample
//-> in mS              1000 / 256 = 3.90625 mS
//-> // Divide this by frequency for higher values (as using integers, add 0.5 and cast to int)

autoDelay waveTableDelay;


// global settings for waves
const uint32_t WAVE_MAX_TIME_S = 600;
bool wavetable_active = false;
bool pulse_active = false;
uint8_t amplitude = 10;
float frequency = 40;

// utility for wave functions
float waveBaseDelay_uS = 0;    // delay for 1Hz
int16_t waveDelayTime_uS = 0;  // calculated delay for {frequency}
uint16_t table_index = 0;
// Timing variables
unsigned long previousMicros = 0;
uint32_t wave_start_time_mS = 0;


// PUMPING VARS
const int empty_time_S = 240;
const int refill_time_S = 240;

bool pump_empty_active = false;
bool pump_refill_active = false;

bool pump_start_time_mS = 0;

// Sampling Vars
bool streaming_active = true;
bool snapshop_active = false;
uint16_t snapshot_timer_mS = 25000;
uint32_t snapshot_starttime_mS;

#define SAMPLE_DELAY_OFFSET 0  // offset in mS

uint16_t sampleRate_Hz = INIT_SAMPLE_RATE_Hz;
//uint32_t sampleDelay_mS = uint32_t(1000 / sampleRate_Hz) - 4;  // added -5 to make the delay just a little shorter, and ensure we get all samples in before they are sent.
uint32_t sampleDelay_mS = uint32_t(1E3 / sampleRate_Hz) - SAMPLE_DELAY_OFFSET;  //
// The function will stop sampling once buffer it full, so this should make more consistant outputs



uint16_t print_rate_Hz = INIT_PRINT_RATE_Hz;
uint32_t print_delay_mS = uint32_t(1000 / print_rate_Hz);

uint8_t num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle

// Maximum sample rate of 200Hz, minimum print rate of 1Hz, so max number of data to be collected is 200 samples x 8 datapoints, 4 bytes per float = 5600 bytes. Very doable data levels on STM32, HOWEVER;
// remember all this data will need to be copied to JSON so lets double that 11200! -> still very doable on STM32!

// but in the future, this must be restricted
// on smaller microcontrollers. Suggest limit of 4, so print rate cant even be less than 4 times sample rate. This can be defined by DATA_ARRAY_SIZE, and will (later) be used to resitrict sample rates and print rates to
// stay within this value, each time one is updated it must be checked for conformity to this against the other.

int arbitaryData = 1;  // just here to provide an incrementing number for demonstration


#define DATA_ARRAY_SIZE 1

uint32_t timestamp_array[DATA_ARRAY_SIZE];
int16_t data_array_one[DATA_ARRAY_SIZE];
float data_array_two[DATA_ARRAY_SIZE];
float data_array_three[DATA_ARRAY_SIZE];

void init_arrays() {
  for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
    timestamp_array[i] = 0;
    data_array_one[i] = 0;
    data_array_two[i] = 0;
    data_array_three[i] = 0;
  }
}

int16_t samples_written = 0;


// Include headers that need to pull from globals right at the end
// Include headers that stateMachine needs before stateMachine
#include "wavetables.h"
#include "stateMachine.h"  // State machine pulls from globals and headers describing hardware functions
#include "jsonReporter.h"
#include "utilityFunctions.h"

