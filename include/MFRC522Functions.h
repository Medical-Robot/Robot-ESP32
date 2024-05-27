#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 0
#define SS_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

//============================================================================================//

byte *readRFID()
{
  static byte cardUID[4] = {0, 0, 0, 0}; // Stocăm UID-ul cardului aici

  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return NULL; // Nu există card nou
  }

  // Selectăm unul dintre carduri
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return NULL; // Eroare la citirea cardului
  }

  // Copiem UID-ul cardului în variabila noastră
  for (byte i = 0; i < 4; i++)
  {
    cardUID[i] = mfrc522.uid.uidByte[i];
  }

  // Oprim comunicarea cu cardul
  mfrc522.PICC_HaltA();

  // Returnăm UID-ul cardului
  return cardUID;
}

//============================================================================================//