#include <iostream>
#include "cfop.hpp"
#include "cubimg.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << cubimg::Version::version_app << '\n';
        std::cout << "Usage: cubimg algorithm" << '\n';
        std::cout << "Example: cubimg \"R U R' U'\"" << std::endl;
        return 1;
    }

    cubimg::Cube::Cube<3> cube;
    if (std::string(argv[1]) == "-pll") {
        cubimg::Engine::GpEngine gp("test_cube", "png");
        gp.setOutputDir("cfop");
        for (const auto& [name, setup] : cubimg::CFOP::pll_setup) {
            cube.resetCubeState();
            cube.applyAlgo(setup);
            gp.setOutputFilename(name);
            gp.setCube(cube);
            gp.draw();
        }
    } else {
        cube.applyAlgo(argv[1]);
        cube.printCubeInColor();
    }

    return 0;
}