#include "src/servo/MyServo.h"
#include "src/servo/ServoManager.h"
#include "src/util/collections/ArduinoList.h"
#include "src/manipulator/ManipulatorArm.h"
#include "src/manipulator/Manipulator.h"
#include "src/util/types/vector/Vector3.h"
#include "ESP8266TimerInterrupt.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define GRAB_ACTION_NAME "GRAB"
#define RELEASE_ACTION_NAME "RELEASE"
#define SQUEEZE_ACTION_NAME "SQUEEZE"
#define DECOMPRESS_ACTION_NAME "DECOMPRESS"
#define MOVE_ACTION_NAME "MOVE"

#define SERVO_MANAGER_PIN D8

#define USING_TIM_DIV1                true           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              false            // for longest timer but least accurate. Default

ServoManager servoManager(SERVO_MANAGER_PIN, ArduinoList<uint8_t>().chainAdd(D5).chainAdd(D6).chainAdd(D7));

MyServo manipulatorBaseServo(0, 2, AngleOverloadAction::REPEAT);
MyServo firstArmServo(1);
MyServo secondArmServo(2, 1, AngleOverloadAction::CLAMP, 90);
MyServo thirdArmServo(3, 1, AngleOverloadAction::CLAMP, 90);
MyServo grabServo(4);

ManipulatorArm firstManipulatorArm(&firstArmServo, 105); // 10.5
ManipulatorArm secondManipulatorArm(&secondArmServo, 65); // 6.5
ManipulatorArm thirdManipulatorArm(&thirdArmServo, 65); // 6.5

Manipulator* manipulator;

int str;

ESP8266Timer ITimer;

// Структура для приема данных
typedef struct ManipulatorControlPanelMessage {
  String action;
  float movementX;
  float movementY;
  float movementZ;
} ManipulatorControlPanelMessage;
// Callback-функция, которая вызывается при приеме данных
void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
  ManipulatorControlPanelMessage receivedData;
  memcpy(&receivedData, data, len);
  if (receivedData.action == MOVE_ACTION_NAME) {
    Serial.println(receivedData.movementZ);
    manipulator->move(Vector3(0, 0, receivedData.movementZ)*10);
  } else if (receivedData.action == GRAB_ACTION_NAME) {
    manipulator->grab();
  } else if (receivedData.action == RELEASE_ACTION_NAME) {
    manipulator->release();
  }
}

void IRAM_ATTR timerHandler() {
  servoManager.mainLoop();
  // Print information about the board and timers
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    yield();
    Serial.println();
  }
    // Инициализируем Wi-Fi как режим Station
  WiFi.mode(WIFI_STA);
  // Инициализируем ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Ошибка инициализации ESP-NOW");
    return;
  }
  // Устанавливаем роль получателя
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  // Регистрируем callback-функцию OnDataRecv для обработки принятых данных
  esp_now_register_recv_cb(onDataRecv);
  
  ITimer.attachInterruptInterval(40, timerHandler);

  servoManager.attachServo(&manipulatorBaseServo);
  servoManager.attachServo(&firstArmServo);
  servoManager.attachServo(&secondArmServo);
  servoManager.attachServo(&thirdArmServo);
  servoManager.attachServo(&grabServo);
  firstArmServo.setServoAngle(90);
  secondArmServo.setServoAngle(90);
  thirdArmServo.setServoAngle(90);
  firstArmServo.setInterpolationMode(true);
  secondArmServo.setInterpolationMode(true);
  thirdArmServo.setInterpolationMode(true);
  manipulator = new Manipulator(
    &manipulatorBaseServo, 
    ArduinoList<ManipulatorArm*>().chainAdd(&firstManipulatorArm).chainAdd(&secondManipulatorArm).chainAdd(&thirdManipulatorArm), 
    &grabServo
  );
}

void loop() {
  str = Serial.read(); // переменная val равна полученной команде
  if (str == '0') thirdArmServo.setServoAngle(0);
  else if (str == '1') thirdArmServo.setServoAngle(90);
  else if (str == '2') thirdArmServo.setServoAngle(180);
  else if (str == '3') thirdArmServo.setServoAngle(-90);
  // if (thirdArmServo.checkIfServoCanRotateOnAngle(90)) Serial.println("ok");
  else if (str == 'g') manipulator->grab(); 
  else if (str == 'r') manipulator->release();
  else if (str == 'w') manipulator->move(Vector3(1,0,0)); 
  else if (str == 'a') manipulator->move(Vector3(0,-1,0)); 
  else if (str == 's') manipulator->move(Vector3(-1,0,0)); 
  else if (str == 'd') manipulator->move(Vector3(0,1,0)); 
  else if (str == 'e') manipulator->move(Vector3(0,0,1)); 
  else if (str == 'q') manipulator->move(Vector3(0,0,-1)); 
  else if (str == 'm') { 
    Serial.print(manipulator->getCurrentPosition().getX());
    Serial.print(" ");
    Serial.print(manipulator->getCurrentPosition().getY());
    Serial.print(" ");
    Serial.println(manipulator->getCurrentPosition().getZ());
  } else if (str == 'n') {
    Serial.println(firstArmServo.getServoAngle());
    Serial.println(secondArmServo.getServoAngle());
    Serial.println(thirdArmServo.getServoAngle());
  }
}