#include "soil_sensor.h"
#include <Arduino.h>
#include "pins.h"

void soilSensorBegin() {
  pinMode(SENSOR_PIN, INPUT);
}

SoilReading soilRead() {
  SoilReading r;
  r.raw = analogRead(SENSOR_PIN); // 0..4095
  r.pct = (r.raw / 4095.0f) * 100.0f;
  return r;
}
