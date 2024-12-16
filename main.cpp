#include <iostream>

#include "./sketch/src/util/math/ArduinoVectorMath.h"
#include "./sketch/src/util/types/vector/Vector2.h"

int main(int argc, char const *argv[])
{
    Vector2 vec = ArduinoVectorMath::makeVectorFromLengthAndAngle(1, 45);

    std::cout << vec.getX() << " "<< vec.getY()<< std::endl;

    return 0;
}
