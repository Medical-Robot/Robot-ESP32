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

#include <DabbleESP32.h>
#include <Path.h>
#include <Map.h>
#include <intersectionSteeringLogic.h>
#include <SerialPortSteeringController.h>
#include <dataStructures.h>
#include <BLEDevice.h>

#define BLACK_COLOR_THRESHOLD 0.50f

#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN 36
#define LINE_SENSOR_2_PIN 39
#define LINE_SENSOR_3_PIN 34
#define LINE_SENSOR_4_PIN 35
#define LINE_SENSOR_5_PIN 32

#define MAX_MOTOR_SPEED 255

// #define EMERGENCY_BREAK_MAX_DISTANCE_FROM_OBSTACLE_CM 10;
// #define DISTANCE_SENSOR_TRIG_PIN 12
// #define DISTANCE_SENSOR_ECHO_PIN 13

Path checkPointPath;
CheckPointDirection checkpointDirection;
// ComandaMedicamente comandaMedicamente;
// Map mapPathCheckpoint;
int onDestination = 0;
std::vector<Checkpoint> checkPoints;
std::vector<PathCheckpoint> pathCheckPoints;
bool isRemoteControlMode = false;

const float PID_Kp = 1.0f;
float maxSpeed = 0.35f;

int linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN, LINE_SENSOR_5_PIN};
LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
Point2D linePosition;

SoftwareSerial serial_soft1(4, 5); // RX TX 16, 17 NU MERG
SoftwareSerialPortSteeringController steeringController(serial_soft1, 255.0f, 0.0f, -255.0f);

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    286.0f, 194.0f, 267.0f, 368.0f, 332.0f};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    3211.0f, 3013.0f, 3040.0f, 3265.0f, 3358.0f};

void printCheckpoints(std::vector<Checkpoint> checkPoints)
{

  for (int i = 0; i < checkPoints.size(); i++)
  {

    Serial.print("Checkpoint ids:");
    Serial.println(checkPoints[i].id);
    Serial.print("back_id: ");
    Serial.println(checkPoints[i].back_id);

    Serial.print("front_id: ");
    Serial.println(checkPoints[i].front_id);

    Serial.print("left_id: ");
    Serial.println(checkPoints[i].left_id);

    Serial.print("right_id: ");
    Serial.println(checkPoints[i].right_id);
  }
}

void setMapFromCloud(int previousCheckPoint, int nextCheckPoint, int destination)
{
  checkPoints = preiaComandaNoua();
  mapPathCheckpoint.getCheckPoints().clear();
  printCheckpoints(checkPoints);
  mapPathCheckpoint.addCheckPoint(checkPoints);
  mapPathCheckpoint.printCheckpoints();
  mapPathCheckpoint.setPreviousCheckPoint(previousCheckPoint);
  mapPathCheckpoint.setNextCheckPoint(nextCheckPoint);

  checkPointPath = mapPathCheckpoint.findPath(destination);
}

void setMap()
{
  Checkpoint checkPoint;

  checkPoint.id = 1;
  checkPoint.front_id = 2;
  checkPoint.back_id = 0;
  checkPoint.left_id = 0;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 2;
  checkPoint.front_id = 4;
  checkPoint.back_id = 1;
  checkPoint.left_id = 0;
  checkPoint.right_id = 3;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 3;
  checkPoint.front_id = 0;
  checkPoint.back_id = 2;
  checkPoint.left_id = 0;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 4;
  checkPoint.front_id = 6;
  checkPoint.back_id = 2;
  checkPoint.left_id = 5;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 5;
  checkPoint.front_id = 0;
  checkPoint.back_id = 4;
  checkPoint.left_id = 0;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 6;
  checkPoint.front_id = 0;
  checkPoint.back_id = 4;
  checkPoint.left_id = 0;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  mapPathCheckpoint.setPreviousCheckPoint(1);
  mapPathCheckpoint.setNextCheckPoint(2);

  checkPointPath = mapPathCheckpoint.findPath(5);
}

// static float getFrontObstacleDistance_cm()
// {
//   // calculations were made in centimeters
//   static uint32_t pulseInTimeout_us = (uint32_t)((200.0f / 34300.0f) * 1000000.0f);

//   float duration;
//   float measured_distance = 0.0f;
//   float estimated_distance = 0.0f;
//   float estimated_distance_sensor1 = 400.0f, estimated_distance_sensor2 = 400.0f, estimated_distance_sensor3 = 400.0f;

//   digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   // Sets the trigPin on HIGH state for 10 micro seconds
//   digitalWrite(DISTANCE_SENSOR_TRIG_PIN, HIGH);
//   delayMicroseconds(10); // This pin should be set to HIGH for 10 μs, at which point the HC­SR04 will send out an eight cycle sonic burst at 40 kHZ
//   digitalWrite(DISTANCE_SENSOR_TRIG_PIN, LOW);
//   // Reads the echoPin, returns the sound wave travel time in microseconds
//   duration = (float)(pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH, pulseInTimeout_us));
//   // Calculating the distance
//   measured_distance = duration * 0.034321f / 2.0f;

//   if (measured_distance <= 0.0f)
//   {
//     measured_distance = 400.0f;
//   }

//   measured_distance = MIN(measured_distance, 400.0f);

//   return measured_distance;
// }

void BluetoothControl()
{
  if (GamePad.isStartPressed())
  {
    isRemoteControlMode = !isRemoteControlMode; // Toggle mode
    delay(300);                                 // Debounce delay
  }
  if (isRemoteControlMode)
  {
    if (GamePad.isUpPressed())
    {
      steeringController.write(maxSpeed, 1.0f, 1.0f);
      Serial.println("UP!");
    }
    else if (GamePad.isDownPressed())
    {
      steeringController.write(maxSpeed, -1.0f, -1.0f);
      Serial.println("DOWN!");
    }
    else if (GamePad.isLeftPressed())
    {
      steeringController.write(maxSpeed, -1.0f, 1.0f);
      Serial.println("LEFT!");
    }
    else if (GamePad.isRightPressed())
    {
      steeringController.write(maxSpeed, 1.0f, -1.0f);
      Serial.println("RIGHT!");
    }
    else
    {
      steeringController.write(0.0f, 0.0f, 0.0f);
    }
    delay(50);
  }
}

void setup()
{
  Dabble.begin("PharmaLinkerCar");
  Serial.begin(9600);
  serial_soft1.begin(9600);
  
  connectingTo();
  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  // pinMode(DISTANCE_SENSOR_TRIG_PIN, OUTPUT); // Trigger pin set to output
  // pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);  // Echo pin set to input

  lineSensors.setPins(linesensors_pins, TOTAL_LINE_SENSORS);
  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);
  setMapFromCloud(1, 2, 6);
  std::vector<Robot> robots = preiaRobot();
  // setMap();
}

float rotateTreshold = 0.5f;
float speed = maxSpeed;
float right_track_speed_cercentage = 1.0f;
float left_track_speed_cercentage = 1.0f;
float PID_out_right, PID_out_left;
Point2D middleLineMax, middleLineMin;
float blackLinePositionX, blackLinePositionY;
int echeckpoint_direction_error;
String toArduino;
float frontObstacleDistance = 0.0f;
float estimated_distance_sensor = 400.0f;

void loop()
{
  Dabble.processInput();

  if (GamePad.isStartPressed())
  {
    isRemoteControlMode = !isRemoteControlMode;
    delay(300); 
  }

  if (isRemoteControlMode)
  {
    BluetoothControl();
  }
  else
  {

    for (size_t i = 0; i < 20; i++)
    {
      lineSensors.read();
      delay(10);
    }

    lineSensors.read();
    middleLineMax = lineSensors.getMaxValue();
    middleLineMin = lineSensors.getMinValue();

    if (middleLineMin.y >= BLACK_COLOR_THRESHOLD && middleLineMax.y >= BLACK_COLOR_THRESHOLD)
    {

      if (checkPointPath.reachedDestination() && onDestination == 0)
      {
        onDestination = 1;
        Serial.print('\t');
        Serial.print("Destination reached");
        retreatBeforeCheckpoint(speed, steeringController, lineSensors, BLACK_COLOR_THRESHOLD);
        // echeckpoint_direction_error = 1;
        if (comandaMedicamente.parmacieCheckpointId == checkPointPath.getDestinationCheckpointId())
        {
          // arrived at the pharmacy
          steeringController.write(0.0f, 0.0f, 0.0f);
        }
        else if (comandaMedicamente.destinationCheckpointId == checkPointPath.getDestinationCheckpointId())
        {
          steeringController.write(0.0f, 0.0f, 0.0f);
          // arrived at the pacient
          /*do someting when arrived at the pacient*/
          // TO DO: ADD AND COMPARE THE UID WHICH EVERY CARD HAS
        }
      }

      Serial.print('\t');
      Serial.print("PathCheckpoint detected: ");
      Serial.println(checkPointPath.getNextCheckPoint().id);

      checkpointDirection = checkPointPath.getNextDirection();
      switch (checkpointDirection)
      {
      case CheckPointDirection::FRONT1:
        middleLineMax.x = 0.0f;
        break;
      case CheckPointDirection::BACK:
        Serial.print("\t Rotate");
        rotate(speed, steeringController, lineSensors, BLACK_COLOR_THRESHOLD);
        break;
      case CheckPointDirection::LEFT:
        Serial.print("\t going left");
        takeLeft(speed, steeringController, lineSensors, BLACK_COLOR_THRESHOLD);

        break;
      case CheckPointDirection::RIGHT:
        Serial.print("\t going right");
        takeRight(speed, steeringController, lineSensors, BLACK_COLOR_THRESHOLD);

        break;
      case CheckPointDirection::NONE:
        // error or reached destination
        // echeckpoint_direction_error = 1;
        break;
      default:
        middleLineMax.x = 0.0f;
        break;
      }

      checkpointDirection = checkPointPath.getNextDirection();
      checkPointPath.goNextCheckPoint();
    }
    else
    {
      speed = maxSpeed;
    }

    if (echeckpoint_direction_error == 1)
    {
      speed = 0.0f;
    }

    blackLinePositionX = middleLineMax.x;
    blackLinePositionY = middleLineMax.y;
    Serial.print("Max Posx:" + String(blackLinePositionX));
    Serial.print('\t');
    Serial.print("Max Posy:" + String(blackLinePositionY));
    Serial.print('\t');
    Serial.print("Min Posx:" + String(middleLineMin.x));
    Serial.print('\t');
    Serial.print("Min Posy:" + String(middleLineMin.y));
    Serial.print('\t');

    /*
    Pos_x: -1   Left: -1    Right: +1
    */

    if (blackLinePositionX < 0.0f)
    {
      PID_out_right = 1.0f;
      if (blackLinePositionX <= (-rotateTreshold))
      {
        PID_out_left = (blackLinePositionX + rotateTreshold) * 2.0f;
      }
      else
      {
        PID_out_left = ((rotateTreshold) + blackLinePositionX) * 2.0f;
      }
    }
    else
    {
      PID_out_left = 1.0f;
      if (blackLinePositionX <= (rotateTreshold))
      {
        PID_out_right = (rotateTreshold - blackLinePositionX) * 2.0f;
      }
      else
      {
        PID_out_right = ((-blackLinePositionX) + rotateTreshold) * 2.0f;
      }
    }

    PID_out_right = PID_out_right * PID_Kp;
    PID_out_left = PID_out_left * PID_Kp;

    right_track_speed_cercentage = PID_out_right;
    left_track_speed_cercentage = PID_out_left;

    left_track_speed_cercentage = MIN(left_track_speed_cercentage, 1.0f);
    left_track_speed_cercentage = MAX(left_track_speed_cercentage, -1.0f);
    right_track_speed_cercentage = MIN(right_track_speed_cercentage, 1.0f);
    right_track_speed_cercentage = MAX(right_track_speed_cercentage, -1.0f);

    Serial.print("left_track:" + String(left_track_speed_cercentage));
    Serial.print('\t');
    Serial.print("right_track:" + String(right_track_speed_cercentage));

    Serial.println();

    // toArduino = String(speed) + ";"+ String(left_track_speed_cercentage) + ";" + String(right_track_speed_cercentage);
    // serial_soft1.print(toArduino);
    if (onDestination == 0)
    {
      steeringController.write(speed, left_track_speed_cercentage, right_track_speed_cercentage);
    }
    else
    {
      steeringController.write(0.0f, 0.0f, 0.0f);
    }

    delay(5);
    // steeringController.write(1.0f, 1.0f, 1.0f);
  }
}