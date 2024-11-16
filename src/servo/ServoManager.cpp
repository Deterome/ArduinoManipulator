#include "ServoManager.h"

void ServoManager::mainLoop() {
    currentTimeInMicros = micros(); 
    if (currentTimeInMicros - pulseStartTime >= PULSE_DELAY_FOR_SERVO_MICROS) {
        digitalWrite(this->servoManagerPin, HIGH);
        pulseStartTime = currentTimeInMicros;
        pulsed = 0;
        if (currentTimeInMicros - this->managedServoChangedTime >= DEFAULT_DELAY_FOR_MANAGED_SERVO_UPDATE) {
            this->managedServoChangedTime = currentTimeInMicros;
            changeManagedServo((this->managedServoId + 1) % this->attachedServos.size());
        }
    } else {
        if (currentTimeInMicros - pulseStartTime >= this->currentPulseWidth && !pulsed) {
            pulsed = 1;
            digitalWrite(this->servoManagerPin, LOW);
        }
    }
}

ServoManager::ServoManager(uint8_t servoManagerPin, const ArduinoList<uint8_t>& addressPinsOfServos) : ServoManager(servoManagerPin, DEFAULT_INTERRUPTION_FREQUENCY_IN_MICROS, addressPinsOfServos) {}

ServoManager::ServoManager(uint8_t servoManagerPin, uint16_t angleCheckingFrequencyInMicroseconds, const ArduinoList<uint8_t>& addressPinsOfServos) {
    this->servoManagerPin = servoManagerPin;
    this->servoAddressPins = addressPinsOfServos;
    pinMode(servoManagerPin, OUTPUT);
    configureTimer(angleCheckingFrequencyInMicroseconds);
}

ServoManager::~ServoManager() {
    this->resetTimer();
}

void ServoManager::attachServo(MyServo* servo) {
    this->attachedServos.add(servo);

    if (this->attachedServos.size() == 1) {
        changeManagedServo(0);
    }
}

void ServoManager::detachServo(MyServo* servo) {
    this->attachedServos.remove(servo);

    if (this->attachedServos.size() == 0) {
        this->managedServoId = 0;
        this->currentPulseWidth = PULSE_DELAY_FOR_SERVO_MICROS;
    }
}

void ServoManager::configureTimer(uint16_t angleCheckingFrequencyInMicroseconds)
{
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = angleCheckingFrequencyInMicroseconds; // прерывание будет срабатывать о данному значению
    TCCR2B |= (1 << WGM22); // режим CTC
    TCCR2B |= (1 << CS20); // предделитель 1
    TIMSK2 |= (1 << OCIE2A); // разрешить прерывание по совпадению
    sei();
}

void ServoManager::resetTimer() {
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    sei();
}

void ServoManager::setServoAddress(uint8_t servoDemultiplexerPortId) {
    for (uint8_t addressPinId = 0; addressPinId < this->servoAddressPins.size(); addressPinId++) {
        uint8_t divider = addressPinId == 0 ? 1 : addressPinId * 2;
        pinMode(this->servoAddressPins[addressPinId], (servoDemultiplexerPortId/divider)%2);
    }
}

void ServoManager::changeManagedServo(uint8_t servoId) {
    this->managedServoId = servoId;
    this->setServoAddress(this->attachedServos[servoId]->getPinOnDemultiplexer());
    this->currentPulseWidth = attachedServos[this->managedServoId]->getServoAngleInMicros();
}
