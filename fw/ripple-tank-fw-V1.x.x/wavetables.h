/*

Wavetables.h contains full implementation for a wavetable driven audio synth


*/

// sine slightly offset from 0 to give op-amp buffer headroom
// ---------------------------- WAVETABLE ----------------------------
// 12-bit unipolar sine table (0..4095)

#include "tables.h"


void wavetable_setup(){

}


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
  // Serial.print("Base Delay uS: ");
  // Serial.println(waveBaseDelay_uS);
}


void select_wavetable(float frequency = 1) {
  if (frequency <= low_table_Hz) {
    currentTable = LOW_HZ_TABLE;
    tableSize = low_table_size;
  } else if (frequency > low_table_Hz && frequency <= mid_table_Hz) {
    currentTable = MID_HZ_TABLE;
    tableSize = mid_table_size;
  } else if (frequency > mid_table_Hz) {
    currentTable = HIGH_HZ_TABLE;
    tableSize = high_table_size;
  } else {  // default // error? // default to low table
    currentTable = LOW_HZ_TABLE;
    tableSize = low_table_size;
    Serial.println("ERROR");
  }
}



// Polling implementation, fine on its own but not if other functions (like long serial prints) are using up loop time
void run_wavetable() {
  uint16_t tableVal = 0;
  if (waveTableDelay.microsDelay(waveDelayTime_uS)) {
    if (table_index >= tableSize) table_index = 0;  // reset this first as it will avoid indexes going OOB
                                                    // Serial.println(table_index);
    if (currentTable == VH_HZ_TABLE) {
      tableVal = pgm_read_word(&sineTable_Vhigh[table_index]);
    } else if (currentTable == HIGH_HZ_TABLE) {
      tableVal = pgm_read_word(&sineTable_high[table_index]);
    } else if (currentTable == MID_HZ_TABLE) {
      tableVal = pgm_read_word(&sineTable_mid[table_index]);
    } else if (currentTable == LOW_HZ_TABLE) {
      tableVal = pgm_read_word(&sineTable_low[table_index]);
    } else {
      tableVal = 255;  //stable mid scale output will highlight this error
    }
    analogWrite(wave_pin, tableVal);
    table_index++;
  }
}





