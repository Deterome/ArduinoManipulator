#pragma once

template <typename T, typename K>
class Pair
{
private:
    T firstEl;
    K secondEl;
public:
    Pair() {}

    Pair(const Pair<T, K>& pair) : firstEl(pair.firstEl), secondEl(pair.secondEl) {}

    Pair(const T& firstEl, const K& secondEl) : firstEl(firstEl), secondEl(secondEl) {}

    Pair<T, K>& operator =(const Pair<T, K>& pair) {
        this->firstEl = pair.firstEl;
        this->secondEl = pair.secondEl;
        return *this;
    }

    T getFirst() const {
        return this->firstEl;
    }

    K getSecond() const {
        return this->secondEl;
    }

    void setFirst(T element) {
        this->firstEl = element;
    }

    void setSecond(K element) {
        this->secondEl = element;
    }
};