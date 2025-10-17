#ifndef CUBIMG_CLI_H
#define CUBIMG_CLI_H

#include "CLI/CLI.hpp"
#include <array>
#include <string>
#include <vector>
#include <filesystem>

namespace cubimg::CLI {

// Default argument values
constexpr int DEFAULT_ORDER = 3;
constexpr int DEFAULT_WIDTH = 400;
constexpr int DEFAULT_HEIGHT = 400;
constexpr int DEFAULT_ELEVATION = 60;
constexpr int DEFAULT_AZIMUTH = 120;
constexpr double DEFAULT_LINEWIDTH = 3.0;
constexpr double DEFAULT_REFLECT_DIST = 2.25;
constexpr std::string_view DEFAULT_COLORS_UFR = "white,green,red";

constexpr std::string_view DEFAULT_ENGINE_PATH = "gnuplot";

constexpr std::array ALLOWED_FORMATS = {
    "pdf", "png", "svg", "tex", "tikz"
};
constexpr std::array ALLOWED_ALGO_SETS = {
    "pll", "oll", "f2l"
};

// Trim leading & trailing spaces around a string.
std::string trimSpaces(const std::string& s);
// Split a string using given separator character and trim spaces around each item.
std::vector<std::string> splitAndTrimSpaces(const std::string& s, char sep);

struct Options {
    std::string engine_path;
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
    double reflection_distance = DEFAULT_REFLECT_DIST;
    std::string reflection;
    std::string colors_UFR;
};

void setup_app(::CLI::App& app, Options& opts);

}

#endif // CUBIMG_CLI_H