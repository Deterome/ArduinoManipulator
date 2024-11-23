#include <avr/io.h>
#include <avr/interrupt.h>
#include "src/servo/MyServo.h"
#include "src/servo/ServoManager.h"
#include "src/util/collections/ArduinoList.h"

#define SERVO_MANAGER_PIN 11

MyServo firstServo(0);
MyServo secondServo(1);
MyServo thirdServo(2);
MyServo fourthServo(3);
ServoManager servoManager(SERVO_MANAGER_PIN, ArduinoList<uint8_t>().chainAdd(4).chainAdd(5));

ISR(TIMER2_COMPA_vect) {
    servoManager.mainLoop();
}

void setup() {
  servoManager.attachServo(&firstServo);
  servoManager.attachServo(&secondServo);
  servoManager.attachServo(&thirdServo);
  servoManager.attachServo(&fourthServo);
}

void loop() {
  firstServo.setServoAngle(90);
  secondServo.setServoAngle(180);
  thirdServo.setServoAngle(20);
  fourthServo.setServoAngle(180);
}