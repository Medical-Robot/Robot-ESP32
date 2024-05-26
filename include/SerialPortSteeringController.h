#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>


class HardwareSerialPortSteeringController
{
public:
    HardwareSerialPortSteeringController(HardwareSerial& serialport, float max_forward_raw_value, float stand_still_speed_raw_value, float max_backward_raw_value) : serialPort_hardware(serialport){
    }
    void write(float speed_percentage, float left_track_percentage, float right_track_percentage){
        this->serialPort_hardware.print(speed_percentage);
        this->serialPort_hardware.print(';');
        this->serialPort_hardware.print(left_track_percentage);
        this->serialPort_hardware.print(';');
        this->serialPort_hardware.print(right_track_percentage);
        this->serialPort_hardware.print('\r\n');
    }
private:
    HardwareSerial& serialPort_hardware;
};


class SoftwareSerialPortSteeringController
{
public:
    SoftwareSerialPortSteeringController(SoftwareSerial& serialport, float max_forward_raw_value, float stand_still_speed_raw_value, float max_backward_raw_value) : serialPort_software(serialport){
    }
    void write(float speed_percentage, float left_track_percentage, float right_track_percentage){
        this->serialPort_software.print(speed_percentage);
        this->serialPort_software.print(';');
        this->serialPort_software.print(left_track_percentage);
        this->serialPort_software.print(';');
        this->serialPort_software.print(right_track_percentage);
        this->serialPort_software.print("\r\n");
    }
private:
    SoftwareSerial& serialPort_software;
};
