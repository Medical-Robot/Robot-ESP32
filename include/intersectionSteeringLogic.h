#pragma once
#include <SteeringController.h>
#include <LineSensors.h>

template <class T> static void takeRight(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    uint32_t startTime;
    //speed = 0.8f;
    int loopDelayMs = 10;
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
    steeringController.write(speed, 1.0f, -1.0f);
        delay(2000);
/*
    lineSensor.read();
    while (lineSensor.getMinValue().y >= blackThreshold) {
        steeringController.write(speed, -1.0f, -1.0f);
        delay(loopDelayMs);
        lineSensor.read();
    }




    while (!(lineSensor.getMaxValue().y >= blackThreshold && lineSensor.getMaxValue().x < -0.8f) && !(lineSensor.getMinValue().y < blackThreshold && lineSensor.getMinValue().x < -0.8f)) {
        steeringController.write(speed, 1.0f, 0.0f);
        delay(loopDelayMs);
        lineSensor.read();
    }
    */
}


template <class T> static void takeLeft(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    steeringController.write(speed, -1.0f, 1.0f);
        delay(2000);
}

template <class T> static void rotate(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    steeringController.write(speed, -1.0f, -1.0f);
        delay(6000);
}
