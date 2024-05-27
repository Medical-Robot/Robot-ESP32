#include <Arduino.h>
// #include <vector>
// #include <queue>
#include <LineSensors.h>
#include <MFRC522Functions.h>
#include <Path.h>
#include <Map.h>
#include <intersectionSteeringLogic.h>
#include <SteeringController.h>
#include <dataStructures.h>

#if defined(ESP32)
#include <FireBaseFunctions.h>
#endif

//============================================================================================//

#define BLACK_COLOR_THRESHOLD 0.5f
#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN 36
#define LINE_SENSOR_2_PIN 39
#define LINE_SENSOR_3_PIN 34
#define LINE_SENSOR_4_PIN 35
#define LINE_SENSOR_5_PIN 32

const float PID_Kp = 1.0f;
int linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN, LINE_SENSOR_5_PIN};
LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
Point2D linePosition;
Path checkPointPath;
CheckPointDirection checkpointDirection;

SteeringController steeringController(255.0f, 0.0f, -255.0f);

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    1312.0f, 691.0f, 1006.0f, 1378.0f, 1491.0f};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    4095.0f, 4067.0f, 3923.0f, 4095.0f, 4095.0f};

//============================================================================================//

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
  checkPoint.front_id = 3;
  checkPoint.back_id = 1;
  checkPoint.left_id = 7;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 3;
  checkPoint.front_id = 8;
  checkPoint.back_id = 2;
  checkPoint.left_id = 0;
  checkPoint.right_id = 5;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 5;
  checkPoint.front_id = 0;
  checkPoint.back_id = 0;
  checkPoint.left_id = 3;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 7;
  checkPoint.front_id = 0;
  checkPoint.back_id = 0;
  checkPoint.left_id = 0;
  checkPoint.right_id = 2;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  checkPoint.id = 8;
  checkPoint.front_id = 0;
  checkPoint.back_id = 3;
  checkPoint.left_id = 0;
  checkPoint.right_id = 0;
  mapPathCheckpoint.addCheckPoint(checkPoint);

  mapPathCheckpoint.setPreviousCheckPoint(1);
  mapPathCheckpoint.setNextCheckPoint(2);

  checkPointPath = mapPathCheckpoint.findPath(8);
}

//============================================================================================//

void setup()
{
  Serial.begin(9600);

  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  delay(4);

  connectingTo();

  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  lineSensors.setPins(linesensors_pins, TOTAL_LINE_SENSORS);
  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);

  // setMap();
}

//============================================================================================//

float speed = 0.5f;
float right_track_speed_percentage = 1.0f;
float left_track_speed_percentage = 1.0f;
float PID_out_right, PID_out_left;
Point2D middleLineMax, middleLineMin;
int echeckpoint_direction_error = 0;

void loop()
{
  byte *cardUID = readRFID();
  std::vector<Checkpoint> checkPoints;
  preiaComandaNoua();
  checkPoints = mapPathCheckpoint.getCheckPoints();

  // sendStateToCLoud();

  lineSensors.read();
  middleLineMax = lineSensors.getMaxValue();
  middleLineMin = lineSensors.getMinValue();
  /*
      Serial.print("Max Posx:" + String(middleLineMax.x));
      Serial.print('\t');
      Serial.print("Max Posy:" + String(middleLineMax.y));
      Serial.print('\t');
      Serial.print("Min Posx:" + String(middleLineMin.x));
      Serial.print('\t');
      Serial.print("Min Posy:" + String(middleLineMin.y));
      Serial.print('\t');


      Pos_x: -1   Left: -1    Right: +1
  */

  if (middleLineMin.y >= BLACK_COLOR_THRESHOLD)
  {
    Serial.print('\t');
    Serial.print("PathCheckpoint detected");
    if (checkPointPath.reachedDestination())
    {
      // echeckpoint_direction_error = 1;
      if (comandaMedicamente.parmacieCheckpointId == checkPointPath.getDestinationCheckpointId())
      {
        // arrived at the pharmacy
        steeringController.write(0.0f, 0.0f, 0.0f);
        /*scan medicaments*/
        byte *cardUID = readRFID();

        // go to the pacient
        mapPathCheckpoint.setNextCheckPoint(checkPointPath.getNextCheckPoint().id);
        mapPathCheckpoint.setPreviousCheckPoint(checkPointPath.getPreviousCheckpoint().id);
        checkPointPath = mapPathCheckpoint.findPath(comandaMedicamente.destinationCheckpointId);
      }
      else if (comandaMedicamente.destinationCheckpointId == checkPointPath.getDestinationCheckpointId())
      {
        steeringController.write(0.0f, 0.0f, 0.0f);
        // arrived at the pacient
        sendStateToCLoud(ComandaMedicamenteStatus::STATUS_COMPLETED);
        /*do someting when arrived at the pacient*/
        // TO DO: ADD AND COMPARE THE UID WHICH EVERY CARD HAS
      }
    }

    checkpointDirection = checkPointPath.getNextDirection();
    checkPointPath.goNextCheckPoint();

    switch (checkpointDirection)
    {
    case CheckPointDirection::FRONT:
      middleLineMax.x = 0.0f;
      break;
    case CheckPointDirection::BACK:
      break;
    case CheckPointDirection::LEFT:
      takeLeft(speed, steeringController, lineSensors, BLACK_COLOR_THRESHOLD);
      break;
    case CheckPointDirection::RIGHT:
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
  }
  else
  {
    speed = 0.5f;
  }

  if (echeckpoint_direction_error == 1)
  {
    speed = 0.0f;
  }

  if (middleLineMax.x < 0.0f)
  {
    PID_out_right = 1.0f;
    if (middleLineMax.x <= (-0.5f))
    {
      PID_out_left = (middleLineMax.x + 0.5f) * 2.0f;
    }
    else
    {
      PID_out_left = ((0.5f) + middleLineMax.x) * 2.0f;
    }
  }
  else
  {
    PID_out_left = 1.0f;
    if (middleLineMax.x <= (0.5f))
    {
      PID_out_right = (0.5f - middleLineMax.x) * 2.0f;
    }
    else
    {
      PID_out_right = ((-middleLineMax.x) + 0.5f) * 2.0f;
    }
  }

  PID_out_right = PID_out_right * PID_Kp;
  PID_out_left = PID_out_left * PID_Kp;

  right_track_speed_percentage = PID_out_right;
  left_track_speed_percentage = PID_out_left;

  left_track_speed_percentage = MIN(left_track_speed_percentage, 1.0f);
  left_track_speed_percentage = MAX(left_track_speed_percentage, -0.9f);
  right_track_speed_percentage = MIN(right_track_speed_percentage, 1.0f);
  right_track_speed_percentage = MAX(right_track_speed_percentage, -0.90f);

  // Serial.print("left_track:" + String(left_track_speed_percentage));
  // Serial.print('\t');
  // Serial.print("right_track:" + String(right_track_speed_percentage));

  Serial.println();

  steeringController.write(speed, left_track_speed_percentage, right_track_speed_percentage);
}
