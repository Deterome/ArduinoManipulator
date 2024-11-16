#include <avr/io.h>
#include <avr/interrupt.h>
#include "src/servo/MyServo.h"
#include "src/servo/ServoManager.h"
#include "src/util/collections/ArduinoList.h"

#define SERVO_MANAGER_PIN 11

MyServo firstServo(0);
ServoManager servoManager(SERVO_MANAGER_PIN, ArduinoList<uint8_t>().chainAdd(4).chainAdd(5).chainAdd(6).chainAdd(7));

ISR(TIMER2_COMPA_vect) {
    servoManager.mainLoop();
}

void setup() {
  servoManager.attachServo(&firstServo);
}

void loop() {
  firstServo.setServoAngle(90);
}