#pragma once

template <typename T>
class Optional {
private:
    T* element = nullptr;
public:
    Optional(const Optional& optional) {
        if (optional.element != nullptr) this->element = new T(*optional.element);
    }
    Optional() {}
    Optional(const T* element) {
        if (element != nullptr) Optional(&element); 
    }
    Optional(const T& element) {
        this->element = new T(element);
    }
    ~Optional() {
        delete this->element;
    }
    Optional& operator = (const Optional& optional) {
        if (optional.element != nullptr) this->element = new T(*optional.element);
    }
    T& get() {
        return *element;
    }
    bool isExist() {
        return element != nullptr;
    }
};

