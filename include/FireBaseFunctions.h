#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson-v6.21.5.h>
#include <Path.h>
#include <Map.h>
#include <dataStructures.h>

#define WIFI_SSID "qwert"
#define WIFI_PASSWORD "31415926"
// #define WIFI_SSID "Off Limits2"
// #define WIFI_PASSWORD "J7s2tzvzKzva"
#define FIREBASE_HOST "https://firestore.googleapis.com"
#define FIREBASE_PROJECT_ID "pharmalinker2"
// #define FIREBASE_COLLECTION_PATH "map"
// #define FIREBASE_COLLECTION_PATH "comenzi"
#define FIREBASE_API_KEY "AIzaSyDhs5wDN_j-u1jwX426LxOtRF2tJPmeJeg"
// #define FIREBASE_API_KEY "AIzaSyBtzlSH-aTnBYVfFXhPXAWFhXn1A8Cd6U4"
#define FIREBASE_DATABASE_URL "https://pharmalinker2-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson firebaseJson;
Map mapPathCheckpoint;
bool signupOK = false;
ComandaMedicamente comandaMedicamente;
WiFiClientSecure client;
HTTPClient https;

//============================================================================================//

String getFirestoreData(const String &documentPath)
{
    client.setInsecure(); // Use with caution, should verify certificates in production

    String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/map" + "/" + documentPath + "?key=" + FIREBASE_API_KEY;

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

//============================================================================================//

String sendStatusToFirestore(const String &documentPath, const String &data)
{
    client.setInsecure();

    String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/comenzi" + "/" + documentPath + "?key=" + FIREBASE_API_KEY;

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    int httpCode = https.PUT(data); // Sending data as a string

    if (httpCode > 0)
    {
        Serial.printf("Data sent successfully, HTTP response code: %d\n", httpCode);
    }
    else
    {
        Serial.printf("PUT request failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
}

//============================================================================================//

std::vector<Checkpoint> preiaComandaNoua()
{
    int i;
    String documentPath;
    String jsonStr;
    Checkpoint checkpoint;
    DeserializationError error;
    StaticJsonDocument<1024> jsonDoc;
    JsonObject root, fields;

    for (i = 1; i <= 6; i++)
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

        case 4:
            checkpoint.id = 4;
            documentPath = "4";
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

    return mapPathCheckpoint.getCheckPoints(); // returning the map from the Date Base
    Serial.println("Succesfull reading from DB.");
}

//============================================================================================//

void connectingTo()
{
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
}

//============================================================================================//

void sendStateToCLoud(ComandaMedicamenteStatus status)
{
    // Convertirea codului de eroare într-un string
    String statusParsed;
    static uint32_t startTime = millis();
    String documentPath = "1";
    String jsonStr;
    StaticJsonDocument<1024> comenziDoc;

    switch (status)
    {
    case ComandaMedicamenteStatus::STATUS_NONE:
        statusParsed = "STATUS_NONE";
        break;
    case ComandaMedicamenteStatus::STATUS_INPROGRESS:
        statusParsed = "STATUS_INPROGRESS";
        break;
    case ComandaMedicamenteStatus::STATUS_FAILED:
        statusParsed = "STATUS_FAILED";
        break;
    case ComandaMedicamenteStatus::STATUS_COMPLETED:
        statusParsed = "STATUS_COMPLETED";
        break;
    }

    comenziDoc["status"] = statusParsed;

    if ((millis() - startTime) >= 5000)
    {
        startTime = millis();
        serializeJson(comenziDoc, jsonStr);
        sendStatusToFirestore(documentPath, jsonStr);
        Serial.printf("Data sent with succes to DB.");
    }
}

//============================================================================================//

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

//============================================================================================//

void comandaCompletata()
{
}

//============================================================================================//
