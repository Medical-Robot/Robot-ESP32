#include <Arduino.h>
#define ENABLE_ARDUINO 1
#define MOTORS_RIGHT_IN1_PIN1 33
#define MOTORS_RIGHT_IN2_PIN2 25
#define MOTORS_LEFT_IN3_PIN1 26
#define MOTORS_LEFT_IN4_PIN2 27

#include <SteeringController.h>

#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#endif

SteeringController steeringController(255.0f, 0.0f, -255.0f);

  void setup()
{
  Serial.begin(9600);
  /*
  while (!Serial) {
    delay(100);
  }*/

  pinMode(MOTORS_RIGHT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_LEFT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN4_PIN2, OUTPUT);

  analogWrite(MOTORS_RIGHT_IN1_PIN1, 255);
  analogWrite(MOTORS_RIGHT_IN2_PIN2, 255);
  analogWrite(MOTORS_LEFT_IN3_PIN1, 255);
  analogWrite(MOTORS_LEFT_IN4_PIN2, 255);
}


void loop()
{
  Serial.println("backward: ");
  steeringController.write(0.5f, 1.0f, -1.0f);
}