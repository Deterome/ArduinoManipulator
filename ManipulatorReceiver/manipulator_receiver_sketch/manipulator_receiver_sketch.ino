#include <ESP8266WiFi.h>
#include <espnow.h>
#include "./src/util/types/vector/Vector3.h"

#define GRAB_ACTION_NAME "GRAB"
#define RELEASE_ACTION_NAME "RELEASE"
#define SQUEEZE_ACTION_NAME "SQUEEZE"
#define DECOMPRESS_ACTION_NAME "DECOMPRESS"
#define MOVE_ACTION_NAME "MOVE"

// Структура для приема данных
typedef struct ManipulatorControlPanelMessage {
  String action;
  float movementX;
  float movementY;
  float movementZ;
} ManipulatorControlPanelMessage;
// Создаем экземпляр структуры struct_message с именем receivedData
// Callback-функция, которая вызывается при приеме данных
void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
  ManipulatorControlPanelMessage receivedData;
  memcpy(&receivedData, data, len);
  Serial.print("Получены данные: ");
  if (receivedData.action == "MOVE") {
    Serial.print(receivedData.movementX*1000);
    Serial.print(" ");
    Serial.print(receivedData.movementY*1000);
    Serial.print(" ");
    Serial.println(receivedData.movementZ*1000);
  } else {
    String receivedActionMessage;
    memcpy(&receivedActionMessage, data, len);
    Serial.println(receivedActionMessage);
  }
}
void setup() {
  Serial.begin(115200);
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
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
