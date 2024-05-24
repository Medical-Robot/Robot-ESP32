#include <Arduino.h>
#include <QTRSensors.h>

#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#endif

#define LINE_SENSOR_1_PIN 12
#define LINE_SENSOR_2_PIN 13
#define LINE_SENSOR_3_PIN 14
#define LINE_SENSOR_4_PIN 32  
#define LINE_SENSOR_5_PIN 
//#define LINE_SENSOR_IR_PIN 15

#define TOTAL_LINE_SENSORS 4


QTRSensors qtr;

const uint8_t SensorCount = TOTAL_LINE_SENSORS;
uint16_t sensorValues[TOTAL_LINE_SENSORS];

const uint8_t linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN};



void setup(){
    Serial.begin(9600);
    
    pinMode(LINE_SENSOR_1_PIN, INPUT);
    pinMode(LINE_SENSOR_2_PIN, INPUT);
    pinMode(LINE_SENSOR_3_PIN, INPUT);
    pinMode(LINE_SENSOR_4_PIN, INPUT);
    //pinMode(LINE_SENSOR_5_PIN, INPUT);
    //pinMode(LINE_SENSOR_IR_PIN, OUTPUT);

    

    // configure the sensors
    qtr.setTypeRC();
    qtr.setSensorPins(linesensors_pins, SensorCount);
    //qtr.setEmitterPin(LINE_SENSOR_IR_PIN);
}


void loop(){
  // read raw sensor values
  //qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 2500, where 0 means maximum
  // reflectance and 2500 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(analogRead(linesensors_pins[i]));
    Serial.print('\t');
  }
  Serial.println();

  delay(250);
}