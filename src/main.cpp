#include "cfop.hpp"
#include "cli.hpp"
#include "cubimg.hpp"
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <functional>

int main(int argc, char* argv[]) {
    cubimg::CLI::Options opts;
    ::CLI::App app{"Cube Image Generator"};
    cubimg::CLI::setup_app(app, opts);

    CLI11_PARSE(app, argc, argv);

    // If no arguments provided, show help and version
    if (argc == 1) {
        std::cout << cubimg::Version::version_app << '\n'
                  << app.help() << std::endl;
        return 1;
    }

    // Create GpEngine instance if output file specified
    cubimg::Engine::GpEngine gp;
    if (!opts.output_file.empty()) {
        gp = cubimg::Engine::GpEngine(
            opts.output_file.stem().string(),
            opts.output_file.extension().string().substr(1)
        );
        gp.setEnginePath(opts.engine_path);
        gp.setOutputDir(opts.output_file.parent_path().string());
        gp.setImageSize(opts.width, opts.height);
        gp.setView(opts.elevation, opts.azimuth);
        gp.setReflection(opts.reflection);
        gp.setReflectionDistance(opts.reflection_distance);
        gp.setLineWidth(opts.linewidth);
        gp.setKeepScriptFile(opts.keep_script);
    }

    // Handle algorithm set case
    if (!opts.algo_set.empty()) {
        if (opts.output_file.empty()) {
            std::cerr << "Error: Arg --file/-f must be specified when using arg --algo-set/-s so that CubImg knows the output folder and format." << std::endl;
            return 1;
        }

        // Call the drawing function based on algo set name string
        std::unordered_map<std::string, std::function<void()>> algo_set_map = {
            {"pll", [&gp]() { gp.drawAlgoSet(cubimg::ALGOSET::pll_algos); }}
        };
        if (auto it = algo_set_map.find(opts.algo_set);
            it != algo_set_map.end()
        ) { it->second(); }
    }

    // Handle normal algorithm case
    try {
        if (!opts.algo.empty()) {
            const auto ufr_vector = cubimg::CLI::splitAndTrimSpaces(opts.colors_UFR, ',');
            auto draw_single_algo = [&](auto&& cube) {
                // Setup the cube
                cube.setUFRColors(ufr_vector[0], ufr_vector[1], ufr_vector[2]);

                // Apply algorithm
                if (opts.algo_reverse) {
                    cube.applyAlgoReverse(opts.algo);
                } else {
                    cube.applyAlgo(opts.algo);
                }

                // Output to console or file
                if (!opts.output_file.empty()) {
                    gp.setCube(cube);
                    gp.draw();
                } else {
                    cube.printCubeInColor();
                }
            };

            switch (opts.order) {
                case 2: draw_single_algo(cubimg::Cube::Cube<2>{}); break;
                case 3: draw_single_algo(cubimg::Cube::Cube<3>{}); break;
                case 4: draw_single_algo(cubimg::Cube::Cube<4>{}); break;
                case 5: draw_single_algo(cubimg::Cube::Cube<5>{}); break;
                case 6: draw_single_algo(cubimg::Cube::Cube<6>{}); break;
                case 7: draw_single_algo(cubimg::Cube::Cube<7>{}); break;
                default:
                    std::cerr << "Error: Unsupported cube order " << opts.order << std::endl;
                    return 1;
            }
        }
    } catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }

    return 0;
}