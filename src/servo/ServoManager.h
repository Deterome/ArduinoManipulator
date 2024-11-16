#pragma once

#define DEFAULT_INTERRUPTION_FREQUENCY_IN_MICROS 7 // 7 is working well
#define PULSE_DELAY_FOR_SERVO_MICROS 20000 
#define DEFAULT_DELAY_FOR_MANAGED_SERVO_UPDATE 300000

#include <Arduino.h>
#include <avr/interrupt.h>
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

    void configureTimer(uint16_t angleCheckingFrequencyInMicroseconds);
    void resetTimer();
    void setServoAddress(uint8_t servoDemultiplexerPortId);
    void changeManagedServo(uint8_t servoId);
public:
    
    ServoManager(uint8_t servoManagerPin, const ArduinoList<uint8_t>& addressPinsOfServos);
    ServoManager(uint8_t servoManagerPin, uint16_t angleCheckingFrequencyInMicroseconds, const ArduinoList<uint8_t>& addressPinsOfServos);
    ~ServoManager();

    void attachServo(MyServo* servo);
    void detachServo(MyServo* servo);
    void mainLoop();
};