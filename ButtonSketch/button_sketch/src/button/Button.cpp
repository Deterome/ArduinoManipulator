#include "Button.h"

Button::Button(uint8_t buttonPin) : Button(buttonPin, DEFAULT_BUTTON_ACTIVE_LEVEL) {}

Button::Button(uint8_t buttonPin, bool buttonActiveMode) {
    this->buttonPin = buttonPin;
    this->activeLevel = buttonActiveMode;
}

void Button::updateState() {
    bool currentButtonState = digitalRead(this->buttonPin);
    uint32_t currentTimeInMillis = millis();
    bool isHold = (currentTimeInMillis - this->buttonPressedTimeInMillis > this->holdTimeoutInMillis) && (currentButtonState == this->activeLevel);
    if (accumulatedCountOfClicks != 0 && currentTimeInMillis - this->buttonPressedTimeInMillis > this->timeToAccumulateClicks && currentButtonState != this->activeLevel) { 
        Optional<Runnable> onPressEvent = onPressEventsByCountOfClicks.get(accumulatedCountOfClicks);
        if (onPressEvent.isExist()) onPressEvent.get().run();
        accumulatedCountOfClicks = 0;
    }
    if (currentButtonState == this->activeLevel && this->lastButtonState != this->activeLevel) {
        if (currentTimeInMillis - this->buttonPressedTimeInMillis > this->buttonPressedCooldownInMillis) {
            this->buttonPressedTimeInMillis = currentTimeInMillis;
        } else {
            currentButtonState = !currentButtonState;
        }
    } else if (currentButtonState != this->activeLevel && this->lastButtonState == this->activeLevel) {
        if (!lastHoldState) {
            accumulatedCountOfClicks++;
        }
        this->onReleaseEvent.run();
    } else if (currentButtonState == this->activeLevel && isHold) {
        if (isHoldFuncRepeatable || !this->lastHoldState) {
            if (holdFuncInvokeTimeoutInMillis == 0 || currentTimeInMillis - lastHoldFuncInvokeTimeInMillis > holdFuncInvokeTimeoutInMillis) {
                lastHoldFuncInvokeTimeInMillis = currentTimeInMillis;
                Optional<Runnable> onHoldEvent = onHoldEventsByCountOfClicksBeforeHold.get(accumulatedCountOfClicks);
                if (onHoldEvent.isExist()) onHoldEvent.get().run();
                accumulatedCountOfClicks = 0;
            }
        }
    }
    this->lastButtonState = currentButtonState;
    this->lastHoldState = isHold;
}

void Button::setButtonPressedCooldown(uint16_t buttonPressedCooldownInMillis) {
    this->buttonPressedCooldownInMillis = buttonPressedCooldownInMillis;
}

void Button::setHoldTimeout(uint16_t holdTimeoutInMillis) {
    this->holdTimeoutInMillis = holdTimeoutInMillis;
}

void Button::setHoldFuncRepeatMode(bool isRepeatable) {
    this->isHoldFuncRepeatable = isRepeatable;
}

void Button::setHoldFuncInvokeTimeout(uint32_t holdFuncInvokeTimeoutInMillis) {
    this->holdFuncInvokeTimeoutInMillis = holdFuncInvokeTimeoutInMillis;
}

void Button::setOnPressFunc(void (*onPressFunc)(), uint16_t numberOfClicks) {
    if (numberOfClicks == 0) numberOfClicks = 1;
    this->onPressEventsByCountOfClicks.put(numberOfClicks, Runnable(onPressFunc));
}

void Button::setOnHoldFunc(void (*onHoldFunc)(), uint16_t numberOfClicksBeforeHold) {
    this->onHoldEventsByCountOfClicksBeforeHold.put(numberOfClicksBeforeHold, Runnable(onHoldFunc));
}

void Button::setOnReleaseFunc(void (*onReleaseFunc)()) {
    this->onReleaseEvent = Runnable(onReleaseFunc);
}

void Button::setTimeToAccumulateClicks(uint16_t timeToAccumulateCliks) {
    this->timeToAccumulateClicks = timeToAccumulateCliks;
}
