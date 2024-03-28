#include <Arduino.h>
#include <Wire.h>


void setup() {
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(4); 
  Wire.write("x is ");        
  Wire.endTransmission(); 
}

