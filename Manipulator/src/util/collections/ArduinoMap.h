#pragma once

#include <stdint.h>
#include "../types/Pair.h"
#include "../types/Optional.h"

template <typename KEY, typename T>
class ArduinoMap
{
private:
    struct ArduinoMapElement {
        Pair<KEY, T> pair;
        ArduinoMapElement* lessElement = nullptr;
        ArduinoMapElement* greaterElement = nullptr;
    };
    uint16_t countOfElements = 0;
    ArduinoMapElement* firstElement = nullptr;
public:
    ~ArduinoMap() {
        do {
            ArduinoMapElement* lastElPtr = nullptr;
            ArduinoMapElement* currentElPtr = firstElement;
            while (currentElPtr != nullptr) {
                if (currentElPtr->lessElement != nullptr) {
                    lastElPtr = currentElPtr;
                    currentElPtr = currentElPtr->lessElement;
                }
                else if (currentElPtr->greaterElement != nullptr) {
                    lastElPtr = currentElPtr;
                    currentElPtr = currentElPtr->greaterElement;
                }
                else {
                    if (lastElPtr != nullptr) {
                        if (lastElPtr->lessElement == currentElPtr) lastElPtr->lessElement = nullptr;
                        else lastElPtr->greaterElement = nullptr;
                    }
                    delete currentElPtr;
                    currentElPtr = lastElPtr;
                    lastElPtr = nullptr;
                }
            }
        } while(firstElement != nullptr);
    }

    void put(const KEY& key, const T& element) {
        this->countOfElements++;
        ArduinoMapElement* currentElPtr = firstElement;
        ArduinoMapElement* lastElPtr = nullptr;
        bool isGreater;
        while (currentElPtr != nullptr) {
            if (key == currentElPtr->pair.getFirst()) break;
            lastElPtr = currentElPtr;
            isGreater = key > currentElPtr->pair.getFirst();
            if (isGreater) currentElPtr = currentElPtr->greaterElement;
            else currentElPtr = currentElPtr->lessElement;
        }
        ArduinoMapElement* newElement = new ArduinoMapElement();
        if (currentElPtr != nullptr) {
            newElement->lessElement = currentElPtr->lessElement;
            newElement->greaterElement = currentElPtr->greaterElement;
            delete currentElPtr;
        }
        newElement->pair = Pair<KEY, T>(key, element);
        if (lastElPtr != nullptr) isGreater ? lastElPtr->greaterElement = newElement : lastElPtr->lessElement = newElement;
        else firstElement = newElement;
    }

    Optional<T> get(const KEY& key) {
        ArduinoMapElement* currentElementPtr = firstElement;
        while (currentElementPtr != nullptr) {
            if (key == currentElementPtr->pair.getFirst()) return Optional<T>(currentElementPtr->pair.getSecond());
            currentElementPtr = key < currentElementPtr->pair.getFirst() ? currentElementPtr->lessElement : currentElementPtr->greaterElement;
        }
        return Optional<T>();
    }

    uint32_t size() {
        return this->countOfElements;
    }
};
