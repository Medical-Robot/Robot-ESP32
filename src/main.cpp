#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 9

struct robot_message{
  
};

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial){
      delay(100);
  }
  Serial.println("ESP32 Started");
}

void loop() {

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write("Hello from ESP32!");
  Wire.endTransmission();
  delay(1000);
  
}

