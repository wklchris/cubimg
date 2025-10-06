#include <iostream>
#include "cubimg.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << cubimg::Version::version_app << '\n';
        std::cout << "Usage: cubimg algorithm" << '\n';
        std::cout << "Example: cubimg \"R U R' U'\"" << std::endl;
        return 1;
    }

    using cubimg::Cube::Cube;

    Cube<3> cube;
    cube.applyAlgo(argv[1]);
    cube.printCubeInColor();

    cubimg::Engine::GpEngine gp("test_cube", "svg");
    gp.setCube(cube);
    gp.draw();

    gp.setOutputFormat("png");
    gp.draw();

    return 0;
}