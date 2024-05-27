#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <LineSensors.h>
#include <FireBaseFunctions.h>
#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#endif

#include <Path.h>
#include <Map.h>
#include <intersectionSteeringLogic.h>
#include <SerialPortSteeringController.h>
#include <dataStructures.h>
#include <BLEDevice.h>

// #define EMERGENCY_BREAK_MAX_DISTANCE_FROM_OBSTACLE_CM 10;
// #define DISTANCE_SENSOR_TRIG_PIN 12
// #define DISTANCE_SENSOR_ECHO_PIN 13

SteeringController steeringController();

// static float getFrontObstacleDistance_cm()
// {
//     // calculations were made in centimeters
//     static uint32_t pulseInTimeout_us = (uint32_t)((200.0f / 34300.0f) * 1000000.0f);

//     float duration;
//     float measured_distance = 0.0f;
//     float estimated_distance = 0.0f;
//     float estimated_distance_sensor1 = 400.0f, estimated_distance_sensor2 = 400.0f, estimated_distance_sensor3 = 400.0f;

//     digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
//     delayMicroseconds(2);
//     // Sets the trigPin on HIGH state for 10 micro seconds
//     digitalWrite(DISTANCE_SENSOR_TRIG_PIN, HIGH);
//     delayMicroseconds(10); // This pin should be set to HIGH for 10 μs, at which point the HC­SR04 will send out an eight cycle sonic burst at 40 kHZ
//     digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
//     // Reads the echoPin, returns the sound wave travel time in microseconds
//     duration = (float)(pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH, pulseInTimeout_us));
//     // Calculating the distance
//     measured_distance = duration * 0.034321f / 2.0f;

//     if (measured_distance <= 0.0f)
//     {
//         measured_distance = 400.0f;
//     }

//     measured_distance = MIN(measured_distance, 400.0f);

//     return measured_distance;
// }

void setup()
{
    Serial.begin(9600);
    pinMode(DISTANCE_SENSOR_TRIG_PIN, OUTPUT); // Trigger pin set to output
    pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);  // Echo pin set to input
}

float frontObstacleDistance = 0.0f;
float estimated_distance_sensor = 400.0f;

void loop()
{

    frontObstacleDistance = getFrontObstacleDistance_cm();
    if (frontObstacleDistance <= 10)
    {
        steeringController.write(0, 0, 0);
    }
}