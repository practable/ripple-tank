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

*/

#include "sine.h"
#include <autoDelay.h>

// Constants
const int dacPin = A0;      // DAC0
const int tableSize = 240;  // Number of samples in the sine wave table


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



// Timing variables
unsigned long previousMicros = 0;
uint16_t table_index = 0;


void calc_wave_baseDelay(uint16_t num_entries) {
  waveBaseDelay_uS = 1000000.0 / float(num_entries);
  Serial.print("Base Delay uS: ");
  Serial.println(waveBaseDelay_uS);
}

uint32_t set_frequency(float frequency = 1) {
  waveDelayTime_uS = uint32_t((waveBaseDelay_uS / frequency) + 0.5);  // waveBaseDelay_uS = 3906 previously
  return waveDelayTime_uS;
}


void run_wavetable() {
  if (waveTableDelay.microsDelay(waveDelayTime_uS)) {
    uint16_t tableVal = pgm_read_word(&sineTable[table_index]);
    analogWrite(dacPin, tableVal);
    // Serial.println(tableVal);
    table_index++;
    if (table_index >= tableSize) table_index = 0;
  }
}





void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.print("Wavetable Testing");
  // Fill the sine lookup table with values from 0 to 255
  // for (int i = 0; i < tableSize; i++) {
  //   sineTable[i] = (uint8_t)(amplitude * sin(2 * PI * i / tableSize) + offset);
  //  }

  calc_wave_baseDelay(tableSize);
  analogWriteResolution(10);
  set_frequency(0.1);
}

//int DAC_val = 0;
//int dac_modifyer = 1;



float frequency = 0;

void loop() {

  run_wavetable();


  if (demoDelay.secondsDelay(8)) {
    frequency = frequency + 0.5;
    set_frequency(frequency);
    Serial.println(frequency);
  }
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
