#pragma once

class Runnable
{
private:
    void (*func)() = nullptr;
public:
    Runnable() {}
    Runnable(const Runnable& runnable) : func(runnable.func) {}
    Runnable(void (*func)()) {
        this->func = func;
    }

    Runnable& operator = (const Runnable& runnable) {
        this->func = runnable.func;
        return *this;
    }
    
    void run() {
        if (func != nullptr) this->func();
    }
};
