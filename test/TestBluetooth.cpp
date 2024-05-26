#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Firebase_ESP_Client.h>
#include "SteeringController.h"

#define MOTORS_RIGHT_IN1_PIN1 33
#define MOTORS_RIGHT_IN2_PIN2 25
#define MOTORS_LEFT_IN3_PIN1 26
#define MOTORS_LEFT_IN4_PIN2 27

#define MAX_MOTOR_SPEED 255

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;
SteeringController steeringController(255.0f, 0.0f, -255.0f);



void setUpPinModes()
{
  pinMode(MOTORS_RIGHT_IN1_PIN1,OUTPUT);
  pinMode(MOTORS_RIGHT_IN2_PIN2,OUTPUT);
  
  pinMode(MOTORS_LEFT_IN3_PIN1,OUTPUT);
  pinMode(MOTORS_LEFT_IN4_PIN2,OUTPUT);
}

void setup()
{
  Serial.begin(9600);
  steeringController.attach(MOTORS_LEFT_IN3_PIN1,MOTORS_LEFT_IN4_PIN2, MOTORS_RIGHT_IN1_PIN1,MOTORS_RIGHT_IN2_PIN2);
  setUpPinModes();
  Dabble.begin("MyBluetoothCar"); 
}

float xAxisJoystick;
float yAxisJoystick;

void loop()
{
  Dabble.processInput();
  xAxisJoystick = GamePad.getx_axis();
  yAxisJoystick = GamePad.gety_axis();
  Serial.print("X: ");
  Serial.print(xAxisJoystick);
  Serial.print("y: ");
  Serial.println(yAxisJoystick);

/*
  if (GamePad.isUpPressed()){
    steeringController.write(1.0f, 1.0f, 1.0f);
  }

  if (GamePad.isDownPressed())
  {
    steeringController.write(1.0f, -1.0f, -1.0f);

  }

  if (GamePad.isLeftPressed())
  {
    steeringController.write(1.0f, -1.0f, 1.0f);

  }

  if (GamePad.isRightPressed())
  {
    steeringController.write(1.0f, 1.0f, -1.0f);
  }
*/

}