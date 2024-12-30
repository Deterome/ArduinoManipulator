#pragma once

#define PULSE_DELAY_FOR_SERVO_MICROS 20000 
#define DEFAULT_DELAY_FOR_MANAGED_SERVO_UPDATE 20000

#include <Arduino.h>
#include "MyServo.h"
#include "../util/collections/ArduinoList.h"

class ServoManager {
private:
    uint8_t servoManagerPin;
    uint32_t currentTimeInMicros = 0;
    uint32_t pulseStartTime = 0;
    uint8_t pulsed = 0;
    uint16_t currentPulseWidth = PULSE_DELAY_FOR_SERVO_MICROS;
    uint8_t managedServoId = 0;
    uint32_t managedServoChangedTime = 0;
    ArduinoList<MyServo*> attachedServos;
    ArduinoList<uint8_t> servoAddressPins;

    void setServoAddress(uint8_t servoDemultiplexerPortId);
    void changeManagedServo(uint8_t servoId);
public:
    
    ServoManager(uint8_t servoManagerPin, const ArduinoList<uint8_t>& addressPinsOfServos);
    ServoManager(uint8_t servoManagerPin, uint16_t angleCheckingFrequencyInMicroseconds, const ArduinoList<uint8_t>& addressPinsOfServos);

    void attachServo(MyServo* servo);
    void detachServo(MyServo* servo);
    void mainLoop();
};