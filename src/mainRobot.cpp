#include <Arduino.h>
// #include <vector>
//  #include <queue>
#include <LineSensors.h>
#define ENABLE_ARDUINO 1
#define MOTORS_LEFT_IN1_PIN1 33
#define MOTORS_LEFT_IN2_PIN2 25
#define MOTORS_RIGHT_IN3_PIN1 26
#define MOTORS_RIGHT_IN4_PIN2 27

#include <Path.h>
#include <Map.h>
#include <intersectionSteeringLogic.h>
#include <SteeringController.h>
#include <dataStructures.h>

#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#endif

// #define WIFI_SSID "qwert"
// #define WIFI_PASSWORD "31415926"
#define WIFI_SSID "Off Limits2"
#define WIFI_PASSWORD "J7s2tzvzKzva"
#define FIREBASE_HOST "https://firestore.googleapis.com"
#define FIREBASE_PROJECT_ID "pharmalinker2"
#define FIREBASE_COLLECTION_PATH "map"
// #define FIREBASE_API_KEY "AIzaSyBtzlSH-aTnBYVfFXhPXAWFhXn1A8Cd6U4"
#define FIREBASE_API_KEY "AIzaSyDhs5wDN_j-u1jwX426LxOtRF2tJPmeJeg"
#define FIREBASE_DATABASE_URL "https://pharmalinker2-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define BLACK_COLOR_THRESHOLD 0.5f

// gg
// arduin uno
/*
  White offset
  S1: 75
  S2: 55
  S3: 56
  S4: 74
  S5: 82
*/

/*
  background laborator offset
  S1: 225
  S2: 91
  S3: 101
  S4: 203
  S5: 197
*/

/*
  colored line value:
  S1: 581
  S2: 572
  S3: 557
  S4: 586
  S5: 588
*/

// ESP32
/*
  White offset
  S1: 1312.00
  S2: 691.00
  S3: 1006.00
  S4: 1378.00
  S5: 1491.00
*/

/*
  background laborator offset
  S1: 225
  S2: 91
  S3: 101
  S4: 203
  S5: 197
*/

/*
  colored line value:
  S1: 581
  S2: 572
  S3: 557
  S4: 586
  S5: 588
*/

#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN 36
#define LINE_SENSOR_2_PIN 39
#define LINE_SENSOR_3_PIN 34
#define LINE_SENSOR_4_PIN 35
#define LINE_SENSOR_5_PIN 32

const float PID_Kp = 1.0f;
bool signupOK = false;
unsigned long sendDataPrevMillis = 0;

int linesensors_pins[TOTAL_LINE_SENSORS] = {LINE_SENSOR_1_PIN, LINE_SENSOR_2_PIN, LINE_SENSOR_3_PIN, LINE_SENSOR_4_PIN, LINE_SENSOR_5_PIN};
LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
Point2D linePosition;
Path checkPointPath;
CheckPointDirection checkpointDirection;
ComandaMedicamente comandaMedicamente;
Map mapPathCheckpoint;

SteeringController steeringController(255.0f, 0.0f, -255.0f);

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    1312.0f, 691.0f, 1006.0f, 1378.0f, 1491.0f};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
    4095.0f, 4067.0f, 3923.0f, 4095.0f, 4095.0f};

void setMap()
{
  Checkpoint checkPoint;
  CheckPointDirection direction;
  Map mapPathCheckpoint;

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

String getFirestoreData(const String &documentPath)
{
  WiFiClientSecure client;
  client.setInsecure(); // Use with caution, should verify certificates in production

  HTTPClient https;
  String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/" + FIREBASE_COLLECTION_PATH + "/" + documentPath + "?key=" + FIREBASE_API_KEY;

  https.begin(client, url);
  int httpCode = https.GET();

  String payload;
  if (httpCode > 0)
  {
    payload = https.getString();
  }
  else
  {
    Serial.printf("GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();
  return payload;
}

void preiaComandaNoua()
{
  int i;
  String documentPath;
  String jsonStr;
  Checkpoint checkpoint;
  DeserializationError error;
  StaticJsonDocument<1024> jsonDoc;
  JsonObject root, fields;

  for (i = 1; i <= 8; i++)
  {
    switch (i)
    {
    case 1:
      checkpoint.id = 1;
      documentPath = "1";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 2:
      checkpoint.id = 2;
      documentPath = "2";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 3:
      checkpoint.id = 3;
      documentPath = "3";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 5:
      checkpoint.id = 5;
      documentPath = "5";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 6:
      checkpoint.id = 6;
      documentPath = "6";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 7:
      checkpoint.id = 7;
      documentPath = "7";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;

    case 8:
      checkpoint.id = 8;
      documentPath = "8";
      jsonStr = getFirestoreData(documentPath);
      error = deserializeJson(jsonDoc, jsonStr);
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        break;
      }

      root = jsonDoc.as<JsonObject>();
      fields = root["fields"];

      checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
      checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
      checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
      checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

      mapPathCheckpoint.addCheckPoint(checkpoint);
      break;
    }
  }
  Serial.println("Succesfull reading from DB.");
}

void comandaCompletata()
{
}

void getInitialConfigsFromCloud()
{
  comandaMedicamente.status = ComandaMedicamenteStatus::STATUS_NONE;
  // get map din cloud
  // get robot coordinates din cloud

  if (comandaMedicamente.status == ComandaMedicamenteStatus::STATUS_NONE)
  {
    // preia comanda din cloud
    // preiaComandaNoua();
  }
}

void sendStateToCLoud()
{
  static uint32_t startTime = millis();

  if ((millis() - startTime) >= 5000)
  {
    startTime = millis();
    // send status to server
  }
}

void setup()
{
  Serial.begin(9600);
  /*
  while (!Serial) {
    delay(100);
  }*/

  pinMode(MOTORS_LEFT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_RIGHT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN4_PIN2, OUTPUT);

  analogWrite(MOTORS_LEFT_IN1_PIN1, LOW);
  analogWrite(MOTORS_LEFT_IN2_PIN2, LOW);
  analogWrite(MOTORS_RIGHT_IN3_PIN1, LOW);
  analogWrite(MOTORS_RIGHT_IN4_PIN2, LOW);

  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  lineSensors.setPins(linesensors_pins, TOTAL_LINE_SENSORS);
  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to connect to WiFi");
  }
  else
  {
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
  }

  /* Assign the api key (required) */
  config.api_key = FIREBASE_API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = FIREBASE_API_KEY;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Succes connecting to Fire Base!");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // getInitialConfigsFromCloud();

  // setMap();
}

float speed = 0.5f;
float right_track_speed_cercentage = 1.0f;
float left_track_speed_cercentage = 1.0f;
float PID_out_right, PID_out_left;
Point2D middleLineMax, middleLineMin;

int echeckpoint_direction_error = 0;

void loop()
{
  std::vector<Checkpoint> checkPoints;
  preiaComandaNoua();
  checkPoints = mapPathCheckpoint.getCheckPoints();
  Serial.print("MARIME VECTOR: ");
  Serial.println(checkPoints.size());

  for (int i = 0; i <= 6; i++)
  {
    Serial.print("Checkpoint ids for document path ");
    Serial.print(i + 1);
    Serial.println(":");
    Serial.print("back_id: ");
    Serial.println(checkPoints[i].back_id);
    Serial.print("front_id: ");
    Serial.println(checkPoints[i].front_id);
    Serial.print("left_id: ");
    Serial.println(checkPoints[i].left_id);
    Serial.print("right_id: ");
    Serial.println(checkPoints[i].right_id);
    delay(1000);
  }

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
        steeringController.write(0.0f, 1.0f, 1.0f);
        /*scan medicaments*/

        // go to the pacient
        mapPathCheckpoint.setNextCheckPoint(checkPointPath.getNextCheckPoint().id);
        mapPathCheckpoint.setPreviousCheckPoint(checkPointPath.getPreviousCheckpoint().id);
        checkPointPath = mapPathCheckpoint.findPath(comandaMedicamente.destinationCheckpointId);
      }
      else if (comandaMedicamente.destinationCheckpointId == checkPointPath.getDestinationCheckpointId())
      {
        steeringController.write(0.0f, 1.0f, 1.0f);
        // arrived at the pacient
        /*do someting when arrived at the pacient*/
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

  right_track_speed_cercentage = PID_out_right;
  left_track_speed_cercentage = PID_out_left;

  left_track_speed_cercentage = MIN(left_track_speed_cercentage, 1.0f);
  left_track_speed_cercentage = MAX(left_track_speed_cercentage, -0.9f);
  right_track_speed_cercentage = MIN(right_track_speed_cercentage, 1.0f);
  right_track_speed_cercentage = MAX(right_track_speed_cercentage, -0.90f);

  // Serial.print("left_track:" + String(left_track_speed_cercentage));
  // Serial.print('\t');
  // Serial.print("right_track:" + String(right_track_speed_cercentage));

  Serial.println();

  steeringController.write(speed, left_track_speed_cercentage, right_track_speed_cercentage);
}