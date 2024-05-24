#pragma once
#include <SteeringController.h>
#include <LineSensors.h>

static void takeRight(float speed, SteeringController& steeringController, LineSensors& lineSensor, float blackThreshold){
    uint32_t startTime;
    speed = 0.8f;
    /*
    for (size_t i = 0; i < 2 || lineSensor.getMinValue().y >= blackThreshold; i++)
    {
        lineSensor.read();
        while (lineSensor.getMinValue().y >= blackThreshold) {
            steeringController.write(speed, -1.0f, -1.0f);
            lineSensor.read();
            delay(1);
        }

        while (lineSensor.getMaxValue().x > -0.8f) {
            steeringController.write(speed, 1.0f, -1.0f);
            lineSensor.read();
            delay(1);
        }

        while(lineSensor.getMaxValue().x < 0.8) {
            steeringController.write(speed, 1.0f, 1.0f);
            lineSensor.read();
            delay(1);
        }

        while (lineSensor.getMaxValue().x > -0.8f) {
            steeringController.write(speed, 1.0f, -1.0f);
            lineSensor.read();
            delay(1);
        } 

        while (lineSensor.getMaxValue().y < blackThreshold) {
            steeringController.write(speed, 1.0f, 1.0f);
            lineSensor.read();
            delay(1);
        } 
    }
    */
   /*
    lineSensor.read();
    while (lineSensor.getMinValue().y >= blackThreshold) {
        steeringController.write(speed, +1.0f, +1.0f);
        lineSensor.read();
        delay(1);
    }
    while (lineSensor.getMaxValue().y >= blackThreshold) {
        steeringController.write(speed, 0.0f, -1.0f);
        lineSensor.read();
        delay(1);
    }

    while (lineSensor.getMaxValue().y < blackThreshold) {
        steeringController.write(speed, 1.0f, 1.0f);
        lineSensor.read();
        delay(1);
    }
    */

    lineSensor.read();
    while (lineSensor.getMinValue().y >= blackThreshold) {
        steeringController.write(speed, -1.0f, -1.0f);
        delay(1);
        lineSensor.read();
    }

    while (lineSensor.getMaxValue().y >= blackThreshold) {
        steeringController.write(speed, 1.0f, -1.0f);
        delay(1);
        lineSensor.read();
    }

    delay(50);

    lineSensor.read();
    while(lineSensor.getMaxValue().x < 0.5f || lineSensor.getMaxValue().y <= blackThreshold) {
        steeringController.write(speed, 1.0f, 1.0f);
        delay(1);
        lineSensor.read();
    }

}


static void takeLeft(float speed, SteeringController& steeringController, LineSensors& lineSensor, float blackThreshold){
    do {
        steeringController.write(speed, -1.0f, 1.0f);
        lineSensor.read();
        delay(50);
        
    } while (lineSensor.getMaxValue().y > blackThreshold);
/*
    do {
        steeringController.write(speed, 1.0f, 1.0f);
        lineSensor.read();
        delay(1);
    } while (lineSensor.getMaxValue().y < blackThreshold);
    */
}
