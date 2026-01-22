
#include "globals.h"

#pragma once

void print_info() {
  Serial.print(F("\n{\"model\":\""));
  Serial.print(EXPERIMENT_NAME);
  Serial.print(F("\",\"version\":\""));
  Serial.print(FIRMWARE_VERSION);
  Serial.print(F("\",\"fw-developed-by\":\""));
  Serial.print(DEVELOPER);
  Serial.println(F("\"}"));
}


void sample_data() {
  timestamp_array[samples_written] = millis();
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  ambient_temp[samples_written] = temp_event.temperature;
  ambient_press[samples_written] = pressure_event.pressure;
  ambient_humid[samples_written] = humidity_event.relative_humidity;
  //arbitaryData++;  // just a placeholder to generate moving data
  samples_written++;
  //Serial.println(humidity_event.relative_humidity);
}