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
#include <FireBaseFunctions.h>

void setup()
{
  Serial.begin(9600);

  //   SPI.begin();        // Init SPI bus
  //   mfrc522.PCD_Init(); // Init MFRC522
  //   delay(4);

  connectingTo();
}

void loop()
{
  getDocumentIDs();
  
  delay(10000);
  // sendStateToCLoud(ComandaMedicamenteStatus::STATUS_COMPLETED);
}