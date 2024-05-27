#pragma once
#include <SteeringController.h>
#include <LineSensors.h>

#define HALF_ROTATION_MS 2000

template <class T> static void takeRight(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    uint32_t startTime;
    speed = 0.4f;
    //speed = 0.8f;
    int loopDelayMs = 5;
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
        delay(HALF_ROTATION_MS / 2);
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
    speed = 0.4f;
    steeringController.write(speed, -1.0f, 1.0f);
        delay(HALF_ROTATION_MS / 2);
}

template <class T> static void rotate(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    speed = 0.4f;
    int loopDelayMs = 5;
    steeringController.write(speed, -1.0f, 1.0f);
    delay(HALF_ROTATION_MS);
    lineSensor.read();
    while (!(lineSensor.getMaxValue().y >= blackThreshold && lineSensor.getMaxValue().x < 0.5f)) {
        steeringController.write(speed, -1.0f, 1.0f);
        lineSensor.read();
        delay(loopDelayMs);
    }
}

template <class T> static void retreatBeforeCheckpoint(float speed, T& steeringController, LineSensors& lineSensor, float blackThreshold){
    speed = 0.4f;
    int loopDelayMs = 5;
    lineSensor.read();
    while (lineSensor.getMinValue().y >= blackThreshold) {
        steeringController.write(speed, -1.0f, -1.0f);
        lineSensor.read();
        delay(loopDelayMs);
    }
    
}

