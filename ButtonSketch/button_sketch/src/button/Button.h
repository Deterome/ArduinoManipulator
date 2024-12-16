#pragma once

#include <Arduino.h>
#include "../util/collections/ArduinoMap.h"
#include "../util/types/Runnable.h"

#define DEFAULT_MIN_HOLD_TIME_IN_MILLIS 500
#define DEFAULT_BUTTON_ACTIVE_LEVEL LOW
#define DEFAULT_HOLD_FUNC_INVOKE_TIMEOUT_IN_MILLIS 0
#define DEFAULT_BUTTON_PRESSED_COOLDOWN_IN_MILLIS 200
#define DEFAULT_TIME_TO_ACCUMULATE_CLICKS 400

class Button {
private:
    uint8_t buttonPin;
    bool activeLevel;
    bool lastHoldState = false;
    uint16_t buttonPressedCooldownInMillis = DEFAULT_BUTTON_PRESSED_COOLDOWN_IN_MILLIS;
    uint16_t holdTimeoutInMillis = DEFAULT_MIN_HOLD_TIME_IN_MILLIS;
    uint16_t holdFuncInvokeTimeoutInMillis = DEFAULT_HOLD_FUNC_INVOKE_TIMEOUT_IN_MILLIS;
    uint16_t lastHoldFuncInvokeTimeInMillis = 0;
    uint32_t buttonPressedTimeInMillis = 0;
    bool isHoldFuncRepeatable = false;
    bool lastButtonState = !DEFAULT_BUTTON_ACTIVE_LEVEL;
    ArduinoMap<uint16_t, Runnable> onPressEventsByCountOfClicks;
    ArduinoMap<uint16_t, Runnable> onHoldEventsByCountOfClicksBeforeHold;
    Runnable onReleaseEvent;
    uint16_t accumulatedCountOfClicks = 0;
    uint16_t timeToAccumulateClicks = DEFAULT_TIME_TO_ACCUMULATE_CLICKS;
public:
    Button(uint8_t buttonPin);
    Button(uint8_t buttonPin, bool buttonActiveMode);

    void updateState();
    void setButtonPressedCooldown(uint16_t buttonPressedCooldownInMillis);
    void setHoldTimeout(uint16_t holdTimeoutInMillis);
    void setHoldFuncRepeatMode(bool isRepeatable);
    void setHoldFuncInvokeTimeout(uint32_t holdFuncInvokeTimeoutInMillis);
    void setOnPressFunc(void (*onPressFunc)(), uint16_t numberOfClicks = 1);
    void setOnHoldFunc(void (*onHoldFunc)(), uint16_t numberOfClicksBeforeHold = 0);
    void setOnReleaseFunc(void (*onReleaseFunc)());
    void setTimeToAccumulateClicks(uint16_t timeToAccumulateCliks);
};

