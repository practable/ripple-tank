#include <Arduino.h>

// Constants
const int dacPin = 25;   // DAC1 (GPIO25)
const int tableSize = 256; // Number of samples in the sine wave table
const int sampleRate = 1000;  // How many samples per second

// Variables for sine wave control
float frequency = 1000.0;  // Frequency of the sine wave in Hz
int amplitude = 128;       // Amplitude of the sine wave (0 to 255)
int offset = 128;          // Offset for DAC midpoint (0-255)

// Lookup table for sine wave (values 0 to 255)
uint8_t sineTable[tableSize];

// Timing variables
unsigned long previousMicros = 0;
int tableIndex = 0;

void setup() {
  // Fill the sine lookup table with values from 0 to 255
  for (int i = 0; i < tableSize; i++) {
    sineTable[i] = (uint8_t)(amplitude * sin(2 * PI * i / tableSize) + offset);
  }

  Serial.begin(115200);
}

void loop() {
  // Calculate the time between samples
  unsigned long currentMicros = micros();
  unsigned long elapsedMicros = currentMicros - previousMicros;

  // Adjust sample output timing based on the desired frequency
  if (elapsedMicros >= (1000000 / sampleRate)) {
    previousMicros = currentMicros;

    // Output the sine value from the lookup table to the DAC
    dacWrite(dacPin, sineTable[tableIndex]);

    // Move to the next table index, adjusting for frequency
    tableIndex = (tableIndex + (frequency * tableSize / sampleRate)) % tableSize;
  }

  // You can adjust the frequency or amplitude dynamically by adding serial input or sensor readings here.
}
