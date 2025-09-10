/* Testing the DAC Output of Nano IoT33


- It seems that to use the DAC output DAC0 (board pin A0) we just need to set analogWriteResolution(10) 
    - load on output must be greater than 5k ohms



*/

#include "sine.h"

// Constants
const int dacPin = A0;        // DAC0
const int tableSize = 256;    // Number of samples in the sine wave table
const int sampleRate = 1000;  // How many samples per second

// Variables for sine wave control
float frequency = 1000.0;  // Frequency of the sine wave in Hz
int amplitude = 128;       // Amplitude of the sine wave (0 to 255)
int offset = 128;          // Offset for DAC midpoint (0-255)

// Lookup table for sine wave (values 0 to 255)


// Timing variables
unsigned long previousMicros = 0;
int tableIndex = 0;

void setup() {
  Serial.begin(115200);
  // Fill the sine lookup table with values from 0 to 255
  // for (int i = 0; i < tableSize; i++) {
  //   sineTable[i] = (uint8_t)(amplitude * sin(2 * PI * i / tableSize) + offset);
  //  }
  analogWriteResolution(10);
}

//int DAC_val = 0;
//int dac_modifyer = 1;

uint8_t table_index = 0;


void loop() {



 // analogWrite(dacPin, DAC_val);
 analogWrite(dacPin,  pgm_read_word(&sineTable[table_index]));
 table_index++;

  delayMicroseconds(160);

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
