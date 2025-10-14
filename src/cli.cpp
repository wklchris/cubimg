#include "cli.hpp"
#include "version.hpp"
#include <algorithm>
#include <cctype>
#include <format>
#include <string>
#include <ranges>

namespace cubimg::CLI {

std::string trimSpaces(const std::string& s) {
    auto left = std::find_if_not(s.begin(), s.end(), 
        [](unsigned char c) { return std::isspace(c); }
    );
    auto right = std::find_if_not(s.rbegin(), s.rend(), 
        [](unsigned char c) { return std::isspace(c); }
    ).base();
    return (left < right) ? std::string(left, right) : std::string();
}

std::vector<std::string> splitAndTrimSpaces(const std::string& s, char sep) {
    std::vector<std::string> result;
    auto split_view = s | std::views::split(sep);
    for (const auto& item : split_view) {
        result.push_back(trimSpaces(std::string(item.begin(), item.end())));
    }
    return result;
}

void setup_app(::CLI::App& app, Options& opts) {
    // Positional arg for cube algorithm
    app.add_option("algo", opts.algo,
        "Space-separated algorithm to apply to the cube. Example: \"R U R' U'\""
    ) ->default_val("");

    // Output file arg

    // Concatenate the allowed format string
    std::string formats_str;
    for (const auto& format_ : ALLOWED_FORMATS) {
        if (!formats_str.empty()) { formats_str += ", "; }
        formats_str += format_;
    }
    app.add_option("-f,--file", opts.output_file,
        std::format("Output file path. If empty, print the cube (in expanded view) to screen instead. Allowed format: {}", formats_str)
    ) ->check([&formats_str](const std::string& path) {
        if (path.empty()) return std::string();
        std::filesystem::path p(path);
        std::string ext = p.extension().string();
        // If the extension is not given or not allowed
        if (ext.empty() ||
            std::ranges::find(ALLOWED_FORMATS, ext.substr(1)) == ALLOWED_FORMATS.end()
        ) {
            return std::format("File must have an extension from: {}", formats_str);
        }
        return std::string();
    });
    
    // Cube order arg
    app.add_option("-o,--order", opts.order, "The order of cube (cube size), range in 2 ~ 7")
        ->default_val(DEFAULT_ORDER)
        ->check(::CLI::Range(2, 7));
    // Cube URF color arg
    app.add_option("-I,--color-UFR,--color-init", opts.colors_UFR, 
        "Comma-separated color names on faces U, F, and R. "
        "Useful for cubers who don't use yellow-cross. "
        "Allowed names: yellow, white, red, orange, green, blue."
    ) ->default_val(DEFAULT_COLORS_UFR)
        ->check([](const std::string& s){
        if (std::count(s.begin(), s.end(), ',') != 2) {
            return std::string("Separate the three colors of U, F, R faces with commas. " 
                "Example: \"yellow,red,green\""
            );
        }
        return std::string();
    });
    
    // Algorithm reverse flag
    app.add_flag("-R,--reverse-alg", opts.algo_reverse,
        "Execute the algorithm steps in reverse. Example: R U -> U' R'"
    );
    
    // Image size args
    app.add_option("-W,--width", opts.width, "Image width in pixels (for png/svg only)")
        ->default_val(DEFAULT_WIDTH)
        ->check(::CLI::PositiveNumber);
    app.add_option("-H,--height", opts.height,
        "Image height in pixels (for png/svg only). Set to the same as width if not given."
    ) ->default_val(DEFAULT_HEIGHT)
        ->check(::CLI::PositiveNumber);

    // View angle args
    app.add_option("-E,--elevation", opts.elevation, "View elevation angle (0-90)")
        ->default_val(DEFAULT_ELEVATION)
        ->check(::CLI::Range(0, 90));
    app.add_option("-A,--azimuth", opts.azimuth, "View azimuth angle (90-180)")
        ->default_val(DEFAULT_AZIMUTH)
        ->check(::CLI::Range(90, 180));

    // Show reflection flag
    app.add_option("--reflection", opts.reflection,
        "Cube faces to draw in reflected view. Can only contains upper-/lower-case letter: B, D, and L."
    ) ->check([](const std::string& s) {
        std::string _msg = "Arg --reflection can only contain following upper-/lower-case letters: B, D, L";
        if (s.size() > 3) return _msg;
        for (char face_letter : s) {
            char c = std::toupper(face_letter);
            if (c != 'B' && c != 'D' && c != 'L') {
                return _msg;
            }
        }
        return std::string();
    });

    // Line width option
    app.add_option("-L,--linewidth", opts.linewidth, "Line width for drawing")
        ->default_val(DEFAULT_LINEWIDTH)
        ->check(::CLI::PositiveNumber);

    // Algorithm set option

    std::string algo_set_str;
    for (const auto& set_ : ALLOWED_ALGO_SETS) {
        if (!algo_set_str.empty()) { algo_set_str += ", "; }
        algo_set_str += set_;
    }
    app.add_option("-s,--algo-set", opts.algo_set,
        std::format("Batch drawing a pre-defined set of algorithms: {}", algo_set_str)
    ) ->check([&algo_set_str](const std::string& set) {
            if (std::ranges::find(ALLOWED_ALGO_SETS, set) == ALLOWED_ALGO_SETS.end()) {
                return std::format("Algorithm set must be one of: {}", algo_set_str);
            }
            return std::string();
        });

    // Keep script flag
    app.add_flag("-k,--keep-script", opts.keep_script,
        "Keep the intermediate drawing script file. By default it will be deleted after execution."
    );
    
    // Version information
    app.add_flag("-V,--version", [](size_t count) {
        if (count > 0) {
            std::cout << cubimg::Version::version_app << std::endl;
            exit(0);
        }
    }, "Show version information");
}

}