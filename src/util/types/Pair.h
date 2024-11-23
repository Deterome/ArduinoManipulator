#pragma once

template <typename T>
class Pair
{
private:
    T firstEl;
    T secondEl;
public:
    Pair(const T& firstEl, const T& secondEl) {
        this->firstEl = firstEl;
        this->secondEl = secondEl;
    }

    T getFirst() const {
        return this->firstEl;
    }

    T getSecond() const {
        return this->secondEl;
    }
};