#pragma once
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
std::vector<String> documentIds;
std::vector<Robot> robots;

//============================================================================================//

String getFirestoreData(const String &documentPath)
{
    client.setInsecure(); // Use with caution, should verify certificates in production

    String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/map" + "/" + documentPath + "?key=" + FIREBASE_API_KEY;
    Serial.println(url);
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

    String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/comenzi" + "/" + documentPath + "?updateMask.fieldPaths=status&key=" + FIREBASE_API_KEY;

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    int httpCode = https.PATCH(data); // Sending data as a string

    if (httpCode > 0)
    {
        Serial.printf("Data sent successfully, HTTP response code: %d\n", httpCode);
        if (httpCode == 200)
        {
            Serial.println("Field updated successfully.");
        }
    }
    else
    {
        Serial.printf("PUT request failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
}

//============================================================================================//

// std::vector<Checkpoint> preiaComandaNoua()
// {
//     {
//         documentIds = getDocumentIDs("map");
//         String documentPath;
//         String jsonStr;
//         Checkpoint checkpoint;
//         DeserializationError error;
//         StaticJsonDocument<512> jsonDoc;
//         JsonObject root, fields;

//         for (size_t i = 1; i <= documentIds.size(); i++)
//         {
//             documentPath = documentIds[i];
//             checkpoint.id = 1;
//             jsonStr = getFirestoreData(documentPath);
//             error = deserializeJson(jsonDoc, jsonStr);
//             if (error)
//             {
//                 Serial.print("deserializeJson() failed: ");
//                 Serial.println(error.c_str());
//                 break;
//             }

//             root = jsonDoc.as<JsonObject>();
//             fields = root["fields"];

//             checkpoint.back_id = fields["back_id"]["integerValue"].as<int>();
//             checkpoint.front_id = fields["front_id"]["integerValue"].as<int>();
//             checkpoint.left_id = fields["left_id"]["integerValue"].as<int>();
//             checkpoint.right_id = fields["right_id"]["integerValue"].as<int>();

//             mapPathCheckpoint.addCheckPoint(checkpoint);
//         }
//     }

//     return mapPathCheckpoint.getCheckPoints(); // returning the map from the Date Base
//     Serial.println("Succesfull reading from DB.");
// }

std::vector<Checkpoint> preiaComandaNoua()
{
    int i;
    String documentPath;
    String jsonStr;
    Checkpoint checkpoint;
    DeserializationError error;
    StaticJsonDocument<512> jsonDoc;
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

// ===============================================================================================//

std::vector<Checkpoint> getMap2()
{
    int i;
    String documentPath;
    String jsonStr;
    Checkpoint checkpoint;
    DeserializationError error;
    StaticJsonDocument<512> jsonDoc;
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
    String documentPath = "1";
    String jsonStr;
    StaticJsonDocument<512> comenziDoc;

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

    comenziDoc["fields"]["status"]["stringValue"] = statusParsed;
    serializeJson(comenziDoc, jsonStr);

    sendStatusToFirestore(documentPath, jsonStr);
}

//============================================================================================//

std::vector<String> getDocumentIDs()
{
    client.setInsecure(); // Utilizați cu precauție în producție, ar trebui să verificați certificatele

    String url = String(FIREBASE_HOST) + "/v1/projects/" + FIREBASE_PROJECT_ID + "/databases/(default)/documents/comenzi?pageSize=100&key=" + FIREBASE_API_KEY;

    https.begin(client, url);
    int httpCode = https.GET();

    if (httpCode > 0)
    {
        String payload = https.getString();

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return documentIds; // Returnează lista goală în caz de eroare
        }

        JsonArray documents = doc["documents"].as<JsonArray>();
        for (JsonObject document : documents)
        {
            String name = document["name"];
            String documentId = name.substring(name.lastIndexOf("/") + 1);
            documentIds.push_back(documentId);
        }
    }
    else
    {
        Serial.printf("GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
    return documentIds;
}

//============================================================================================//

std::vector<Robot> preiaRobot()
{
    std::vector<String> documentIDs;
    documentIDs = getDocumentIDs();
    String jsonStr;
    Robot robot;
    DeserializationError error;
    StaticJsonDocument<512> jsonDoc;
    JsonObject root, fields;

    for (const String &documentId : documentIds)
    {
        jsonStr = getFirestoreData(documentId);
        error = deserializeJson(jsonDoc, jsonStr);
        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            continue;
        }

        root = jsonDoc.as<JsonObject>();
        fields = root["fields"];

        robot.destinationCheckpointID = fields["destinationCheckpointID"]["integerValue"].as<int>();
        robot.nextCheckpointID = fields["nextCheckpointID"]["integerValue"].as<int>();
        robot.prevCheckpointID = fields["prevCheckpointID"]["integerValue"].as<int>();
        robot.status = fields["status"]["stringValue"].as<String>();

        Serial.print("Robot id:");
        Serial.println(documentId);

        Serial.print("nextCheckpointID:");
        Serial.println(robot.nextCheckpointID);

        Serial.print("prevCheckpointID:");
        Serial.println(robot.prevCheckpointID);

        Serial.print("destinationCheckpointID:");
        Serial.println(robot.destinationCheckpointID);

        Serial.print("status:");
        Serial.println(robot.status);

        robots.push_back(robot);
    }

    return robots; // returning the map from the Date Base
    Serial.println("Succesfull reading from DB.");
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
