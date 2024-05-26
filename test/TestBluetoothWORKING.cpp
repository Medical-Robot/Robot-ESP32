#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Arduino.h>
#include <DabbleESP32.h>
//#include <Firebase_ESP_Client.h>
#include "SerialPortSteeringController.h"

#define MOTORS_RIGHT_IN1_PIN1 33
#define MOTORS_RIGHT_IN2_PIN2 25
#define MOTORS_LEFT_IN3_PIN1 26
#define MOTORS_LEFT_IN4_PIN2 27

#define MAX_MOTOR_SPEED 255

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;
SoftwareSerial serial_soft1(4, 5); // RX TX 16, 17 NU MERG
SoftwareSerialPortSteeringController steeringController(serial_soft1, 255.0f, 0.0f, -255.0f);


void setup()
{
  Serial.begin(9600);
  serial_soft1.begin(9600);
  Dabble.begin("PharmaLinkerCar"); 
}
float maxSpeed = 0.5f;
float xAxisJoystick;
float yAxisJoystick;

void loop()
{
  Dabble.processInput();
    /*
  xAxisJoystick = GamePad.getx_axis();
  yAxisJoystick = GamePad.gety_axis();

  Serial.print("X: ");
  Serial.print(xAxisJoystick);
  Serial.print("y: ");
  Serial.println(yAxisJoystick);
  */
  

  if (GamePad.isUpPressed()){
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
  else{
    steeringController.write(0.0f, 0.0f, 0.0f);
  }
  delay(50);

}