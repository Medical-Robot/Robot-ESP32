#include <SoftwareSerial.h>
#include <Firebase_ESP_Client.h>
#include <BLEDevice.h>


SoftwareSerial serial_soft1(4,5);
void setup()
{
  serial_soft1.begin(9600);

}



void loop()
{
  serial_soft1.print("1;1;1\r\n");
  delay(2000);

}