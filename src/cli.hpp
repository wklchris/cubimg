#ifndef CUBIMG_CLI_H
#define CUBIMG_CLI_H

#include "CLI/CLI.hpp"
#include <array>
#include <filesystem>

namespace cubimg::CLI {

// Default argument values
constexpr int DEFAULT_ORDER = 3;
constexpr int DEFAULT_WIDTH = 400;
constexpr int DEFAULT_HEIGHT = 400;
constexpr int DEFAULT_ELEVATION = 60;
constexpr int DEFAULT_AZIMUTH = 120;
constexpr double DEFAULT_LINEWIDTH = 3.0;

constexpr std::array ALLOWED_FORMATS = {
    "pdf", "png", "svg", "tex", "tikz"
};
constexpr std::array ALLOWED_ALGO_SETS = {
    "pll"
};

struct Options {
    std::string algo;
    int order = DEFAULT_ORDER;
    bool algo_reverse = false;
    std::filesystem::path output_file;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int elevation = DEFAULT_ELEVATION;
    int azimuth = DEFAULT_AZIMUTH;
    double linewidth = DEFAULT_LINEWIDTH;
    std::string algo_set;
    bool keep_script = false;
};

void setup_app(::CLI::App& app, Options& opts);

}

#endif // CUBIMG_CLI_H