#include <ESP8266WiFi.h>
#include <espnow.h>

#define GRAB_ACTION_NAME "GRAB"
#define RELEASE_ACTION_NAME "RELEASE"
#define SQUEEZE_ACTION_NAME "SQUEEZE"
#define DECOMPRESS_ACTION_NAME "DECOMPRESS"
#define MOVE_ACTION_NAME "MOVE"

// Структура для приема данных
typedef struct manipulatorControlPanelMessage {
  String action;
  String value;
} manipulatorControlPanelMessage;
// Создаем экземпляр структуры struct_message с именем receivedData
manipulatorControlPanelMessage receivedData;
// Callback-функция, которая вызывается при приеме данных
void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
  memcpy(&receivedData, data, sizeof(receivedData));
  Serial.print("Получены данные: ");
  // switch(receivedData.action) {
  //   case GRAB_ACTION_NAME:
  //     // отравка по I2C сигнала манипулятору 'захват'
  //     break;
  //   case RELEASE_ACTION_NAME:
  //     // отравка по I2C сигнала манипулятору 'отпустить'
  //     break;
  //   case SQUEEZE_ACTION_NAME:
  //     // отравка по I2C сигнала манипулятору 'сжатие'
  //     break;
  //   case DECOMPRESS_ACTION_NAME:
  //     // отравка по I2C сигнала манипулятору 'расслабление'
  //     break;
  //   case MOVE_ACTION_NAME:
  //     // отравка по I2C сигнала манипулятору 'движение'
  //     break;
  // }
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
}

void loop() {
  Serial.println("Ok");
}
