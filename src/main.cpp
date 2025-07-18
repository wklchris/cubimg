#include <iostream>
#include "cube.hpp"

int main() {
    cubimg::Cube::Cube<3> cube;
    cube.printCubeText();
    cube.printCubeInColor();
    return 0;
}