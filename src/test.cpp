#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <array>
#include <vector>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include <ArduinoJson-v6.21.5.h>
#define WIFI_SSID "Off Limits2"
#define WIFI_PASSWORD "J7s2tzvzKzva"
#define FIREBASE_HOST "https://firestore.googleapis.com"
#define FIREBASE_PROJECT_ID "pharmalinker2"
#define FIREBASE_COLLECTION_PATH "map"
//#define WIFI_SSID "qwert"
//#define WIFI_PASSWORD "3141592653589"

#define I2C_SCL_PIN 21
#define I2C_SDA_PIN 22

// Firebase project API Key & RTDB URLefine the RTDB URL
#define FIREBASE_API_KEY "AIzaSyDhs5wDN_j-u1jwX426LxOtRF2tJPmeJeg"
#define FIREBASE_DATABASE_URL "https://pharmalinker2-default-rtdb.europe-west1.firebasedatabase.app/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
int intValue;
String stringValue;
float floatValue;
bool signupOK = false;

void setup()
{

  Wire.begin();

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    delay(100); // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("ESP32 Started");

  // Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = FIREBASE_API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = FIREBASE_API_KEY;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

String getFirestoreData(const String& documentPath) {
  WiFiClientSecure client;
  client.setInsecure(); // Use with caution, should verify certificates in production

  HTTPClient https;
 String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/" + FIREBASE_COLLECTION_PATH + "/" + documentPath + "?key=" + FIREBASE_API_KEY;

  https.begin(client, url);
  int httpCode = https.GET();

  String payload;
  if (httpCode > 0) {
    payload = https.getString();
  } else {
    Serial.printf("GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();
  return payload;
}

void SendIntData(int data)
{
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", data))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println(String("REASON: ") + fbdo.errorReason());
    }
  }
}

void SendFloatData(float data)
{
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    // Write an float number on the database path test/int
    if (Firebase.RTDB.setFloat(&fbdo, "test/int", data))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println(String("REASON: ") + fbdo.errorReason());
    }
  }
}

void SendStringData(String data)
{
  sendDataPrevMillis = millis();
  // Write an string on the database path test/int
  if (Firebase.RTDB.setString(&fbdo, "test/int", data))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else
  {
    Serial.println("FAILED");
    Serial.println(String("REASON: ") + fbdo.errorReason());
  }
}

int32_t GetIntData()
{
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, "/test/int"))
    {
      if (fbdo.dataType() == "int")
      {
        intValue = fbdo.intData();
        return intValue;
      }
    }
    else
    {
      Serial.println(fbdo.errorReason());
    }
  }
}

float GetFloatData()
{
  if (Firebase.RTDB.getFloat(&fbdo, "/test/float"))
  {
    if (fbdo.dataType() == "float")
    {
      floatValue = fbdo.floatData();
      return floatValue;
    }
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

String GetStringData()
{
  if (Firebase.RTDB.getString(&fbdo, "/test/float"))
  {
    if (fbdo.dataType() == "string")
    {
      stringValue = fbdo.stringData();
      return stringValue;
    }
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

typedef struct PathCheckpoint {
	int next_checkpoint_id;
	int left_id;	// in
	int right_id;	// out
	int front_id;	// out
	int back_id;	// in
}PathCheckpoint;

void loop()
{
  String documentPath = "1";//1, 2, 3, 5, 7, 8

  String jsonStr = getFirestoreData(documentPath);
  Serial.println(jsonStr);

  StaticJsonDocument<1024> jsonDoc;

  DeserializationError error = deserializeJson(jsonDoc, jsonStr);
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject root = jsonDoc.as<JsonObject>();
  JsonObject fields = root["fields"];

  PathCheckpoint pathCheckpoint;

  pathCheckpoint.back_id = fields["back_id"]["integerValue"].as<int>();
  pathCheckpoint.front_id = fields["front_id"]["integerValue"].as<int>();
  pathCheckpoint.left_id = fields["left_id"]["integerValue"].as<int>();
  pathCheckpoint.right_id = fields["right_id"]["integerValue"].as<int>();

  Serial.print("Checkpoint ids:");
  Serial.print("back_id: ");
  Serial.println(pathCheckpoint.back_id);

  Serial.print("front_id: ");
  Serial.println(pathCheckpoint.front_id);

  Serial.print("left_id: ");
  Serial.println(pathCheckpoint.left_id);

  Serial.print("right_id: ");
  Serial.println(pathCheckpoint.right_id);

}

