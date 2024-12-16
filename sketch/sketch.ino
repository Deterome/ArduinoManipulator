#include <avr/io.h>
#include <avr/interrupt.h>
#include "src/servo/MyServo.h"
#include "src/servo/ServoManager.h"
#include "src/util/collections/ArduinoList.h"
#include "src/manipulator/ManipulatorArm.h"
#include "src/manipulator/Manipulator.h"
#include "src/util/types/vector/Vector3.h"

#define SERVO_MANAGER_PIN 11

ServoManager servoManager(SERVO_MANAGER_PIN, ArduinoList<uint8_t>().chainAdd(4).chainAdd(5).chainAdd(6));

MyServo manipulatorBaseServo(0, 2, AngleOverloadAction::REPEAT);
MyServo firstArmServo(1);
MyServo secondArmServo(2);
MyServo thirdArmServo(3);
MyServo grabServo(4);

ManipulatorArm firstManipulatorArm(&firstArmServo, 3);
ManipulatorArm secondManipulatorArm(&secondArmServo, 2);
ManipulatorArm thirdManipulatorArm(&thirdArmServo, 1);

Manipulator manipulator(
  &manipulatorBaseServo, 
  ArduinoList<ManipulatorArm*>().chainAdd(&firstManipulatorArm).chainAdd(&secondManipulatorArm).chainAdd(&thirdManipulatorArm), 
  &grabServo
);

int str;

ISR(TIMER2_COMPA_vect) {
  servoManager.mainLoop();
}

void setup() {
  Serial.begin(9600);
  servoManager.attachServo(&manipulatorBaseServo);
  servoManager.attachServo(&firstArmServo);
  servoManager.attachServo(&secondArmServo);
  servoManager.attachServo(&thirdArmServo);
  servoManager.attachServo(&grabServo);
}

void loop() {
  str = Serial.read(); // переменная val равна полученной команде

  if (str == 'g') manipulator.grab(); 
  else if (str == 'r') manipulator.release();
  else if (str == 'w') manipulator.move(Vector3(1,0,0)); 
  else if (str == 'a') manipulator.move(Vector3(0,-1,0)); 
  else if (str == 's') manipulator.move(Vector3(-1,0,0)); 
  else if (str == 'd') manipulator.move(Vector3(0,1,0)); 
  else if (str == 'e') manipulator.move(Vector3(0,0,1)); 
  else if (str == 'q') manipulator.move(Vector3(0,0,-1)); 
  else if (str == 'm') { 
    Serial.print(manipulator.getCurrentPosition().getX());
    Serial.print(" ");
    Serial.print(manipulator.getCurrentPosition().getY());
    Serial.print(" ");
    Serial.println(manipulator.getCurrentPosition().getZ());
  } else if (str == 'n') { 
    Serial.print(firstManipulatorArm.getVectorizedArm().getX());
    Serial.print(" ");
    Serial.print(firstManipulatorArm.getVectorizedArm().getY());
    Serial.print("; ");
    Serial.print(secondManipulatorArm.getVectorizedArm().getX());
    Serial.print(" ");
    Serial.print(secondManipulatorArm.getVectorizedArm().getY());
    Serial.print("; ");
    Serial.print(thirdManipulatorArm.getVectorizedArm().getX());
    Serial.print(" ");
    Serial.println(thirdManipulatorArm.getVectorizedArm().getY());
  }
}