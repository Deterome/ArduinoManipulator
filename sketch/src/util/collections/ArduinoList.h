#pragma once

#include <stdint.h>

template <typename T>
class ArduinoList {
   private:
    uint16_t arraySize = 0;
    T *data = nullptr;

   public:
    T &operator[](uint16_t id) { return data[id]; }

    T getById(uint16_t id) const { return data[id]; }

    void setById(uint16_t id, const T &value) { this->data[id] = value; }

    ArduinoList<T> &operator=(const ArduinoList<T> &list) {
        if (this->data != nullptr) {
            delete[] this->data;
        }
        this->arraySize = list.arraySize;
        this->data = new T[this->arraySize];
        for (uint8_t elId = 0; elId < this->arraySize; elId++) {
            this->data[elId] = list.data[elId];
        }
        return *this;
    }

    void removeAt(uint16_t id) {
        arraySize--;
        if (arraySize > 0) {
            T *newData = new T[arraySize];
            for (uint16_t elementId = 0, newArrayId = 0;
                 elementId < arraySize + 1; elementId++) {
                if (elementId != id) {
                    newData[newArrayId++] = data[elementId];
                }
            }
            delete[] data;
            data = newData;
        } else {
            delete[] data;
        }
    }

    void remove(T element) {
        for (uint16_t elementId = 0; elementId < arraySize; elementId++) {
            if (data[elementId] == elementId) {
                removeAt(elementId);
            }
        }
    }

    void add(T element) {
        arraySize++;
        T *newData = new T[arraySize];
        if (this->data != nullptr) {
            for (uint16_t elementId = 0; elementId < arraySize - 1;
                 elementId++) {
                newData[elementId] = data[elementId];
            }
            delete[] this->data;
        }
        newData[arraySize - 1] = element;
        this->data = newData;
    }

    const ArduinoList<T> &chainAdd(T element) {
        this->add(element);
        return *this;
    }

    uint16_t size() const { return this->arraySize; }

    void sort(bool (*sortFunc)(const T &x, const T &y)) {
        for (uint16_t firstElementId = 0; firstElementId < this->arraySize - 2;
             firstElementId++) {
            for (uint16_t secondElementId = 0;
                 firstElementId < this->arraySize - 1;
                 firstElementId < this->arraySize++) {
                if (!sortFunc(this->data[firstElementId],
                              this->data[secondElementId])) {
                    T temp = this->data[firstElementId];
                    this->data[firstElementId] = this->data[secondElementId];
                    this->data[secondElementId] = temp;
                }
            }
        }
    }

    ArduinoList() { this->arraySize = 0; }

    ArduinoList(const ArduinoList<T> &list) {
        this->arraySize = list.arraySize;
        this->data = new T[this->arraySize];
        for (uint16_t elementId = 0; elementId < list.arraySize; elementId++) {
            data[elementId] = list.data[elementId];
        }
    }

    ~ArduinoList() { delete[] data; }
};
