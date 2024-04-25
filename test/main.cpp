#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 9
//https://pharmalinker1-default-rtdb.europe-west1.firebasedatabase.app
//web api key: AIzaSyCbKQY0nfPlUcN4ZaptgaCz1iAAxNO5qts

#define EROARE_SENSOR_OBSTACOL 0
#define EROARE_TAG_MEDICAMENT 0
#define EROARE_TAG_PACIENT 0
#define EROARE_SMARTPHONE 0 //MOD MANUAL EROARE
#define EROARE_SISTEM 0
#define EROARE_TRASEU 0
#define EROARE_STARE_ROBOT 0

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

