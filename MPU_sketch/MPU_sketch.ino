#include "MPU6050_6Axis_MotionApps20.h"
#include "./src/util/types/vector/Vector3.h"

#define LED_PIN 13
#define TIME_TO_SEND_DATA_TO_MANIPULATOR_IN_MILLIS 100
#define MPU_IGNORE_VALUE 100
#define ACCELERATION_OF_GRAVITY 9.80665f
#define MEASURE_TIME_IN_MILLIS 10

uint8_t fifoBuffer[45]; 

MPU6050 mpu;

Vector3 movement;
Vector3 currentVelocity;
Vector3 lastAcceleration;
volatile bool isDmpReady = false;
bool isAccidentallyTriggered = true;
uint32_t currentTimeInMillis;
uint32_t nextTimeToMeasureInMillis;

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
  attachInterrupt(0, dmpReady, RISING);
}

void setup() {
  Wire.begin();
  
  setupMpu();

  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
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

void dmpReady() {
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
  Serial.print(movement.getX()*1000);
  Serial.print(" ");
  Serial.print(movement.getY()*1000);
  Serial.print(" ");
  Serial.print(movement.getZ()*1000);
  Serial.println(" ");
  movement = Vector3::ZERO; // удалить
  // в следующий рассчётах нужно учитывать то, что они производятся с какой-то начальной скоростью, которая была найдена в ранних рассчётах
  currentVelocity = newVelocity;
}

void loop() {
  currentTimeInMillis = millis();
  if (isDmpReady && currentTimeInMillis > nextTimeToMeasureInMillis) {
    nextTimeToMeasureInMillis += MEASURE_TIME_IN_MILLIS;
    isDmpReady = false;
    // configuration: finding diviations on each axis. DONT MOVE MPU!
    Vector3 acceleration = getFiltredMpuLinearAccelerationInCmPerSecSqr();
    if (acceleration != Vector3::ZERO) {
      if (isAccidentallyTriggered) {
        isAccidentallyTriggered = false;
      } else {
        processMovement(lastAcceleration, acceleration);
      }
      lastAcceleration = acceleration;
    } else if (lastAcceleration != Vector3::ZERO){
      lastAcceleration = Vector3::ZERO;
      currentVelocity = Vector3::ZERO;
      isAccidentallyTriggered = true;
    }
  }
}