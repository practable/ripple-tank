/*

Wavetables.h contains full implementation for a wavetable driven audio synth


*/

// sine slightly offset from 0 to give op-amp buffer headroom
// ---------------------------- WAVETABLE ----------------------------
// 12-bit unipolar sine table (0..4095)

#include "tables.h"




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
  Serial.print("table size: ");
  Serial.println(tableSize);
}



// Polling implementation, fine on its own but not if other functions (like long serial prints) are using up loop time
void run_wavetable() {
  uint16_t tableVal = 0;
  if (waveTableDelay.microsDelay(waveDelayTime_uS)) {
    if (table_index >= tableSize) table_index = 0;  // reset this first as it will avoid indexes going OOB
    Serial.println(table_index);
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

// PHASE ACCUMULATOR -> DDS (Direct Digital Synthesis) Implementation

// Update frequency function
void setFrequency(float freq) {
  noInterrupts();
  phaseIncrement = (uint32_t)((freq * TABLE_SIZE * (1UL << PHASE_BITS)) / SAMPLE_RATE);
  interrupts();
}



// Function to set up DAQ
void setupDAC() {
  // Enable DAC in 12-bit mode
  analogWriteResolution(10);  // 0–4095

  // Enable DAC clock
  PM->APBCMASK.reg |= PM_APBCMASK_DAC;

  // Connect DAC to GCLK0
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_DAC | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Enable DAC
  DAC->CTRLA.bit.ENABLE = 1;
  while (DAC->STATUS.bit.SYNCBUSY)
    ;

  // Use VDD/2 as reference
  DAC->CTRLB.bit.REFSEL = DAC_CTRLB_REFSEL_AVCC;
  while (DAC->STATUS.bit.SYNCBUSY)
    ;
}

// Function to set up timers & interrupts TC5
// 48 MHz / 16 = 3 MHz
// 3 MHz / 30 kHz = 100
void wavetable_clock_setup() {

  // Enable generic clock for TC4/TC5
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_ID_TC4_TC5 | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Disable TC5
  TC5->COUNT16.CTRLA.reg = 0;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY)
    ;

  // Configure TC5
  TC5->COUNT16.CTRLA.reg =
    TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV16 | TC_CTRLA_WAVEGEN_MFRQ;  // Match Frequency mode
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY)
    ;

  // Set compare value for 30 kHz
  uint32_t compare = (48000000 / 16) / SAMPLE_RATE;
  TC5->COUNT16.CC[0].reg = compare;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY)
    ;

  // Enable interrupt on compare match
  TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  NVIC_EnableIRQ(TC5_IRQn);

  // Enable TC5
  TC5->COUNT16.CTRLA.bit.ENABLE = 1;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY)
    ;
}




// ISR based wavetable function that updates the (SAMD21) DAC directly
void TC5_Handler() {
  if (TC5->COUNT16.INTFLAG.bit.MC0) {
    TC5->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
    // phase += phaseIncrement;
    // uint16_t index = phase >> PHASE_SHIFT;
    wt_ticks++;
    if (wt_ticks >= wt_ticks_per_sample) {
      wt_index++;
      if (wavetable_active) {
        DAC->DATA.reg = pgm_read_word(&sineTable_low[wt_index]) << 2;
        if (wt_index >= TABLE_SIZE) wt_index = 0;  // why not just =0?
      } else {
        DAC->DATA.reg = 0;
      }
    }
  }
}
