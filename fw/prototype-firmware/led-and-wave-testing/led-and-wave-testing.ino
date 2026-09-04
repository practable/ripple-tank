/* Testing the DAC Output of Nano IoT33


- It seems that to use the DAC output DAC0 (board pin A0) we just need to set analogWriteResolution(10) 
    - load on output must be greater than 5k ohms

 - Note, this current implementation output frequency is only accurate to about 30 Hz

  0 to 20 Hz within about +- 0.25 precision,
  after that the frequency gets less accurate due to rounding errors in the timer, 
  could be compensated for but would add significant complexity. 
  Up to 30Hz the maximum offset is below 0.5 Hz. 
  Above 30 Hz it jumps to over 0.5 Hz discrepancy.
 
  This is using a 240 sample wavetable,
  more samples gives greater resolution for the sine wave,
  but less accurate timing for the frequency,
  less samples than this are more accurate for timing but
  start introducing additional harmonics, I would call it unuseable by 120 samples

  To expand this to other frequences, the wavetable should use less samples as frequency increases.

*/

#include "sine.h"
#include <autoDelay.h>

// Digital Pot Stuff
#include <SPI.h>

#include <MCP4151.h>

#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13

//MCP4151 pot(CS, MOSI, MISO, SCK);
// MCP4151 pot(CS, MOSI, MISO, SCK, 4000000, 250000, SPI_MODE0);






uint8_t volume = 10;
float frequency = 40;
uint16_t brightness = 100;

// Constants
const int dacPin = A0;  // DAC0

const int digiPotSelectPin = 10;

const int tableSize = 240;  // Number of samples in the sine wave table
// constants for different sine tables
const int low_table_size = 240;  // up to 30 Hz
const int mid_table_size = 120;  // 30 - 60 Hz
const int high_table_size = 60;  // 60 to 120 Hz

typedef uint8_t enum {
  LOW_HZ_TABLE,
  MID_HZ_TABLE,
  HIGH_HZ_TABLE
} activeTable;


// Variables for sine wave control
//float frequency = 1.0;  // Frequency of the sine wave in Hz
// One complete cycle of wave = {tableSize} samples
//-> for 1Hz operation, 1,000,000 uS / tableSize = {wavedelaytime} 3906.25 uS of delay between each sample
//-> in mS              1000 / 256 = 3.90625 mS
//-> // Divide this by frequency for higher values (as using integers, add 0.5 and cast to int)

autoDelay waveTableDelay;
autoDelay demoDelay;

float waveBaseDelay_uS = 0;    // delay for 1Hz
int16_t waveDelayTime_uS = 0;  // calculated delay for {frequency}
uint16_t table_index = 0;



// Timing variables
unsigned long previousMicros = 0;



//LED Variables
const int ledCtrl = 3;
//int16_t ledPWM = 10;






void setVolume(const uint8_t &volume) {
  digitalWrite(digiPotSelectPin, LOW);
  unsigned char MSb = 0x0;
  unsigned char LSb = volume & 0xFF;  // no value greater than 255, else send 0x00
  uint16_t transmission = MSb << 8 | LSb;
  SPI.transfer16(transmission);
  digitalWrite(digiPotSelectPin, HIGH);
}





uint32_t set_frequency(float frequency = 1) {
  waveDelayTime_uS = uint32_t((waveBaseDelay_uS / frequency) + 0.5);  // waveBaseDelay_uS = 3906 previously
  return waveDelayTime_uS;
}


// Used at startup to calculate the delay time between samples at 1Hz
void calc_wave_baseDelay(uint16_t num_entries) {
  waveBaseDelay_uS = 1000000.0 / float(num_entries);
  Serial.print("Base Delay uS: ");
  Serial.println(waveBaseDelay_uS);
}



void run_wavetable() {
  if (waveTableDelay.microsDelay(waveDelayTime_uS)) {
    Serial.println(waveDelayTime_uS);
    uint16_t tableVal = pgm_read_word(&sineTable[table_index]);
    analogWrite(dacPin, tableVal);
    // Serial.println(tableVal);
    table_index++;
    if (table_index >= tableSize) table_index = 0;
  }
}





void setup() {
  Serial.begin(115200);

  delay(2000);

  // Set Up SPI for digital pot volume control
  pinMode(digiPotSelectPin, OUTPUT);
  SPI.begin();

  Serial.print("Wavetable Testing");
  // Fill the sine lookup table with values from 0 to 255
  // for (int i = 0; i < tableSize; i++) {
  //   sineTable[i] = (uint8_t)(amplitude * sin(2 * PI * i / tableSize) + offset);
  //  }
  pinMode(ledCtrl, OUTPUT);
  calc_wave_baseDelay(tableSize);
  analogWriteResolution(10);
  set_frequency(0.1);
  delay(10);
  set_frequency(frequency);
  setVolume(volume);
  //pot.writeValue(volume);
  analogWrite(ledCtrl, brightness);
}

//int DAC_val = 0;
//int dac_modifyer = 1;





void loop() {

  run_wavetable();



  //

  // analogWrite(dacPin, DAC_val);
  // analogWrite(dacPin,  pgm_read_word(&sineTable[table_index]));
  //table_index++;

  // delayMicroseconds(160);

  // DAC_val += dac_modifyer;

  // if (DAC_val >= 1024) {
  //dac_modifyer = -1;
  //  DAC_val = 0;
  // } else if (DAC_val <= 0) {
  //   dac_modifyer = 1;
  // }

  // Calculate the time between samples
  // unsigned long currentMicros = micros();
  //  unsigned long elapsedMicros = currentMicros - previousMicros;

  // Adjust sample output timing based on the desired frequency
  //  if (elapsedMicros >= (1000000 / sampleRate)) {
  //   previousMicros = currentMicros;

  // Output the sine value from the lookup table to the DAC
  //   dacWrite(dacPin, sineTable[tableIndex]);

  // Move to the next table index, adjusting for frequency
  // tableIndex = (tableIndex + (frequency * tableSize / sampleRate)) % tableSize;
  // }

  // You can adjust the frequency or amplitude dynamically by adding serial input or sensor readings here.
}
