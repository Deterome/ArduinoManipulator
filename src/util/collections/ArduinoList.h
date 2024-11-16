#pragma once

template <typename T>
class ArduinoList
{
private:
    size_t arraySize = 0;
    T* data = nullptr;
public:
    T& operator [](size_t id) {
        return data[id];
    }

    void removeAt(size_t id) {
        arraySize--;
        if (arraySize > 0) {
            T* newData = new T[arraySize];
            for (size_t elementId = 0, newArrayId = 0; elementId < arraySize + 1; elementId++) {
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
        for (size_t elementId = 0; elementId < arraySize; elementId++)
        {
            if (data[elementId] == elementId) {
                removeAt(elementId);
            }
        }
    }

    void add(T element) {
        arraySize++;
        T* newData = new T[arraySize];
        if (data != nullptr) {
            for (size_t elementId = 0; elementId < arraySize - 1; elementId++) {
                newData[elementId] = data[elementId];
            }
            delete[] data;
        }
        newData[arraySize-1] = element;
        data = newData;
    }


    const ArduinoList<T>& chainAdd(T element) {
        this->add(element);
        return *this;
    } 

    size_t size() {
        return this->arraySize;
    }

    ArduinoList() {}

    ArduinoList(const ArduinoList<T>& list) {
        this->arraySize = list.arraySize;
        this->data = new T[this->arraySize];
        for (size_t elementId = 0; elementId < list.arraySize; elementId++)
        {
            data[elementId] = list.data[elementId];
        }
    }

    ~ArduinoList() {
        delete[] data;
    }

};

