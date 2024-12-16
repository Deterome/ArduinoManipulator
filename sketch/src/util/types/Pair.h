#pragma once

template <typename T, typename K>
class Pair
{
private:
    T firstEl;
    K secondEl;
public:
    Pair(const T& firstEl, const K& secondEl) {
        this->firstEl = firstEl;
        this->secondEl = secondEl;
    }

    T getFirst() const {
        return this->firstEl;
    }

    K getSecond() const {
        return this->secondEl;
    }
};