#include "./src/button/Button.h"

#define BUTTON_PIN 3

Button button(BUTTON_PIN);

void onButtonPressed() {
  Serial.println("pressed");
}

void onDoubleClick() {
  Serial.println("double click");
}

void onClickAndHold() {
  Serial.println("click and hold");
}

void onButtonHold() {
  Serial.println("holded");
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setOnPressFunc(onButtonPressed);
  button.setOnHoldFunc(onButtonHold);
  button.setOnPressFunc(onDoubleClick, 2);
  button.setOnHoldFunc(onClickAndHold, 1);
}

void loop() {
  button.updateState();
}
