#include <Arduino.h>
#include <LineSensors.h>
#include <SerialPortSteeringController.h>
#define ENABLE_ARDUINO 1
#define MOTORS_RIGHT_IN1_PIN1 33
#define MOTORS_RIGHT_IN2_PIN2 25
#define MOTORS_LEFT_IN3_PIN1 26
#define MOTORS_LEFT_IN4_PIN2 27
                                    // forward
#define MOTORS_RIGHT_IN1_PIN1 26    // -
#define MOTORS_RIGHT_IN2_PIN2 27    // +
#define MOTORS_LEFT_IN3_PIN1 33     // -
#define MOTORS_LEFT_IN4_PIN2 25     // +

#include <SteeringController.h>

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
#include <SteeringController.h>
#include <dataStructures.h>
#include <BLEDevice.h>

#define BLACK_COLOR_THRESHOLD 0.30f

#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN 36
#define LINE_SENSOR_2_PIN 39
#define LINE_SENSOR_3_PIN 34
#define LINE_SENSOR_4_PIN 35
#define LINE_SENSOR_5_PIN 32

Path checkPointPath;
CheckPointDirection checkpointDirection;
ComandaMedicamente comandaMedicamente;
Map mapPathCheckpoint;
SoftwareSerial serial_soft1(4, 5); // RX TX 16, 17 NU MERG
SoftwareSerialPortSteeringController softwareSerialPortSteeringController(serial_soft1, 255.0f, 0.0f, -255.0f);


const float PID_Kp = 1.0f;

int linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN, LINE_SENSOR_5_PIN};
LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
Point2D linePosition;

SteeringController steeringController(255.0f, 0.0f, -255.0f);

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    1312.0f, 691.0f, 1006.0f, 1378.0f, 1491.0f
  };

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    4095.0f, 4067.0f, 3923.0f, 4095.0f, 4095.0f
  };




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
  checkPoint.left_id = 0;
  checkPoint.right_id = 5;
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

  checkPointPath = mapPathCheckpoint.findPath(3);
}


void setup()
{
  Serial.begin(9600);
  serial_soft1.begin(9600);
  /*
  while (!Serial) {
    delay(100);
  }*/

  pinMode(MOTORS_RIGHT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_LEFT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN4_PIN2, OUTPUT);

  analogWrite(MOTORS_RIGHT_IN1_PIN1, LOW);
  analogWrite(MOTORS_RIGHT_IN2_PIN2, LOW);
  analogWrite(MOTORS_LEFT_IN3_PIN1, LOW);
  analogWrite(MOTORS_LEFT_IN4_PIN2, LOW);

  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  lineSensors.setPins(linesensors_pins, TOTAL_LINE_SENSORS);
  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);
  setMap();
}

float rotateTreshold = 0.5f;
float speed = 0.5f;
float right_track_speed_cercentage = 1.0f;
float left_track_speed_cercentage = 1.0f;
float PID_out_right, PID_out_left;
Point2D middleLineMax, middleLineMin;
float blackLinePositionX, blackLinePositionY;
int echeckpoint_direction_error;



void loop()
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

    if (checkPointPath.reachedDestination())
    {
      Serial.print('\t');
      Serial.print("Destination reached");
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
        //TO DO: ADD AND COMPARE THE UID WHICH EVERY CARD HAS
      }
    }

    Serial.print('\t');
    Serial.print("PathCheckpoint detected: ");
    Serial.println(checkPointPath.getNextCheckPoint().id);

    checkpointDirection = checkPointPath.getNextDirection();
    switch (checkpointDirection)
    {
    case CheckPointDirection::FRONT:
      middleLineMax.x = 0.0f;
      break;
    case CheckPointDirection::BACK:
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
      echeckpoint_direction_error = 1;
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
    speed = 0.5f;
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


  if (blackLinePositionX < 0.0f) {
    PID_out_right = 1.0f;
    if (blackLinePositionX <= (-rotateTreshold)) {
      PID_out_left = (blackLinePositionX + rotateTreshold) * 2.0f;
    }
    else{
      PID_out_left = ((rotateTreshold) + blackLinePositionX) * 2.0f;
    }
  }
  else{
    PID_out_left = 1.0f;
    if (blackLinePositionX <= (rotateTreshold)) {
      PID_out_right = (rotateTreshold - blackLinePositionX) * 2.0f;
    }
    else{
      PID_out_right = ((-blackLinePositionX) + rotateTreshold) * 2.0f;
    }
  }
  
  PID_out_right = PID_out_right * PID_Kp;
  PID_out_left = PID_out_left * PID_Kp;

  right_track_speed_cercentage = PID_out_right;
  left_track_speed_cercentage = PID_out_left;


  left_track_speed_cercentage = MIN(left_track_speed_cercentage, 1.0f);
  left_track_speed_cercentage = MAX(left_track_speed_cercentage, -0.9f);
  right_track_speed_cercentage = MIN(right_track_speed_cercentage, 1.0f);
  right_track_speed_cercentage = MAX(right_track_speed_cercentage, -0.90f);


  Serial.print("left_track:" + String(left_track_speed_cercentage));
  Serial.print('\t');
  Serial.print("right_track:" + String(right_track_speed_cercentage));

  Serial.println();
  steeringController.write(speed, left_track_speed_cercentage, right_track_speed_cercentage);
}