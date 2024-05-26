#include <Arduino.h>
#include <BLEDevice.h>
#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#endif

#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN 36
#define LINE_SENSOR_2_PIN 39
#define LINE_SENSOR_3_PIN 34
#define LINE_SENSOR_4_PIN 35
#define LINE_SENSOR_5_PIN 32

const uint8_t SensorCount = TOTAL_LINE_SENSORS;
uint16_t sensorValues[TOTAL_LINE_SENSORS];

const uint8_t linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN};



void setup(){
    Serial.begin(9600);
    
    pinMode(LINE_SENSOR_1_PIN, INPUT);
    pinMode(LINE_SENSOR_2_PIN, INPUT);
    pinMode(LINE_SENSOR_3_PIN, INPUT);
    pinMode(LINE_SENSOR_4_PIN, INPUT);
    pinMode(LINE_SENSOR_5_PIN, INPUT);

}


void loop(){

    Serial.print(analogRead(LINE_SENSOR_1_PIN));
    Serial.print('\t');
    Serial.print(analogRead(LINE_SENSOR_2_PIN));
    Serial.print('\t');
    Serial.print(analogRead(LINE_SENSOR_3_PIN));
    Serial.print('\t');
    Serial.print(analogRead(LINE_SENSOR_4_PIN));
    Serial.print('\t');
    Serial.print(analogRead(LINE_SENSOR_5_PIN));
    Serial.print('\n');

  delay(250);
}