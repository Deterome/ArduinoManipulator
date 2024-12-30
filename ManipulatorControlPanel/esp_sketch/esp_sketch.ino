#include <ESP8266WiFi.h>
#include <espnow.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "./src/util/types/vector/Vector3.h"
#include "./src/button/Button.h"

#define MPU_IGNORE_VALUE 100
#define ACCELERATION_OF_GRAVITY 9.80665f
#define MEASURE_TIME_IN_MILLIS 10
#define GRAB_ACTION_NAME "GRAB"
#define RELEASE_ACTION_NAME "RELEASE"
#define SQUEEZE_ACTION_NAME "SQUEEZE"
#define DECOMPRESS_ACTION_NAME "DECOMPRESS"
#define MOVE_ACTION_NAME "MOVE"
#define MOVEMENT_SEND_TIME_IN_MILLIS 100

uint8_t fifoBuffer[45]; 

MPU6050 mpu;

Vector3 movement;
Vector3 currentVelocity;
Vector3 lastAcceleration;
volatile bool isDmpReady = false;
bool isAccidentallyTriggered = true;
uint32_t currentTimeInMillis;
uint32_t nextTimeToMeasureInMillis;
Button grabButton(16);
uint8_t manipulatorReceiverMacAddress[] = {0xC4, 0xD8, 0xD5, 0x02, 0x2A, 0x93}; //C4:D8:D5:02:2A:93
uint32_t nextTimeToSendMovementInMillis = 0;

// Структура для приема данных
typedef struct ManipulatorControlPanelMessage {
  String action;
  float movementX;
  float movementY;
  float movementZ;
} ManipulatorControlPanelMessage;

void setupMpu() {
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  mpu.setXAccelOffset(-1124);
  mpu.setYAccelOffset(-206);
  mpu.setZAccelOffset(822);
  mpu.setXGyroOffset(337);
  mpu.setYGyroOffset(-10);
  mpu.setZGyroOffset(-16);
  attachInterrupt(digitalPinToInterrupt(14), dmpReady, RISING);
}

void setupGrabButton() {
  pinMode(16, INPUT);
  grabButton.setOnPressFunc(onGrabButtonPressedOnce);
  grabButton.setOnPressFunc(onGrabButtonPressedTwice, 2);
  grabButton.setOnHoldFunc(onGrabButtonHolded);
  grabButton.setOnHoldFunc(onGrabButtonPressedAndHolded, 1);
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization error");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(manipulatorReceiverMacAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void setup() {
  Wire.begin();
  
  setupMpu();
  setupGrabButton();
  setupWiFi();
  
  Serial.begin(115200);
}

void sendMovementMessageToManipulator(Vector3 vector) {
  sendMessageToManipulator(MOVE_ACTION_NAME, vector);
}

void sendActionMessageToManipulator(String action) {
  sendMessageToManipulator(action, Vector3::ZERO);
}

void sendMessageToManipulator(String action, Vector3 vector) {
  ManipulatorControlPanelMessage message;
  message.action = action;
  message.movementX = vector.getX();
  message.movementY = vector.getY();
  message.movementZ = vector.getZ();
  esp_now_send(manipulatorReceiverMacAddress, (uint8_t *)&message, sizeof(message));

}

void onGrabButtonPressedOnce() {
  sendActionMessageToManipulator(GRAB_ACTION_NAME);
}

void onGrabButtonPressedTwice() {
  sendActionMessageToManipulator(RELEASE_ACTION_NAME);
}

void onGrabButtonHolded() {
  sendActionMessageToManipulator(SQUEEZE_ACTION_NAME);
}

void onGrabButtonPressedAndHolded() {
  sendActionMessageToManipulator(DECOMPRESS_ACTION_NAME);
}

Vector3 getMpuLinearAcceleration() {
  VectorInt16 linearAccel;
  uint16_t fifoCount = mpu.getFIFOCount();
  if (fifoCount == 1024) {
    mpu.resetFIFO();
  } else if (fifoCount >= 42) {
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      VectorFloat gravity;
      Quaternion q;
      VectorInt16 acceleration;
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetAccel(&acceleration, fifoBuffer);
      mpu.dmpGetLinearAccel(&linearAccel, &acceleration, &gravity);
    }
  }
  return Vector3(linearAccel.x, linearAccel.y, linearAccel.z);
}

Vector3 getFiltredMpuLinearAcceleration() {
  return getMpuLinearAcceleration().forEachCoord([](float coord){
    return abs(coord) < MPU_IGNORE_VALUE ? 0 : coord;
  });
}

Vector3 getFiltredMpuLinearAccelerationInCmPerSecSqr() {
  return getFiltredMpuLinearAcceleration().forEachCoord([](float coord){
    return coord / 8192 * ACCELERATION_OF_GRAVITY * 100;
  });
}

IRAM_ATTR void dmpReady() {
  isDmpReady = true;
}

float findIntegral(float deltaTime, float startPoint, float endPoint) {
    return (startPoint + endPoint) * deltaTime / 2;
}

void processMovement(const Vector3& initialAcceleration, const Vector3& currentAcceleration) {
  float deltaTimeInSec = (float)MEASURE_TIME_IN_MILLIS/1000;
  // находим изменение скорости, путём интегрирования ускорения
  Vector3 newVelocity(currentVelocity);
  for (uint8_t coordId = 0; coordId < 3; coordId++) {
    newVelocity.setCoordById(coordId, newVelocity.getCoordById(coordId) + 
      (initialAcceleration.getCoordById(coordId) + currentAcceleration.getCoordById(coordId)) * deltaTimeInSec / 2);
  }
  // находим перемещение, путём интегрирования скорости
  for (uint8_t coordId = 0; coordId < 3; coordId++) {
    movement.setCoordById(coordId, movement.getCoordById(coordId) +
      (currentVelocity.getCoordById(coordId) + newVelocity.getCoordById(coordId)) * deltaTimeInSec / 2);
  }
  // в следующий рассчётах нужно учитывать то, что они производятся с какой-то начальной скоростью, которая была найдена в ранних рассчётах
  currentVelocity = newVelocity;
}

void loop() {
  grabButton.updateState();
  currentTimeInMillis = millis();
  if (currentTimeInMillis > nextTimeToSendMovementInMillis && movement != Vector3::ZERO) {
    nextTimeToSendMovementInMillis = currentTimeInMillis + MOVEMENT_SEND_TIME_IN_MILLIS;
    sendMovementMessageToManipulator(movement);
    movement = Vector3::ZERO;
  }
  if (isDmpReady && currentTimeInMillis > nextTimeToMeasureInMillis) {
    nextTimeToMeasureInMillis += MEASURE_TIME_IN_MILLIS;
    isDmpReady = false;
    // configuration: finding diviations on each axis. DONT MOVE MPU!
    Vector3 acceleration = getFiltredMpuLinearAccelerationInCmPerSecSqr();
    if (acceleration != Vector3::ZERO) {
      if (isAccidentallyTriggered) {
        isAccidentallyTriggered = false;
      } else {
        yield();
        processMovement(lastAcceleration, acceleration);
        yield();
      }
      lastAcceleration = acceleration;
    } else if (lastAcceleration != Vector3::ZERO){
      lastAcceleration = Vector3::ZERO;
      currentVelocity = Vector3::ZERO;
      isAccidentallyTriggered = true;
    }
  }
}