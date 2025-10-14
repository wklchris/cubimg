#ifndef CUBIMG_COLOR_H
#define CUBIMG_COLOR_H

#include <array>
#include <format>
#include <algorithm>
#include <stdexcept>
#include <string_view>

namespace cubimg::Color {

struct CubeColorData {
    using ColorArray = std::array<int, 4>;

    std::string_view name;
    ColorArray rgba;
    std::string_view ansi;
};

// Singleton class CubeColor
class CubeColor {
    using ColorArray = std::array<int, 4>;

public:
    static constexpr std::string_view ansi_reset = "\033[0m";

    // Face colors

    static constexpr CubeColorData white = {
        "white", 
        std::array{255, 255, 255, 255},  // #FFFFFF
        "\033[1;47m"
    };
    static constexpr CubeColorData yellow = {
        "yellow",
        std::array{238, 242, 12, 255},  // #EEF20C
        "\033[48;5;226m"
    };
    static constexpr CubeColorData red = {
        "red",
        std::array{243, 41, 41, 255},  // #F32929
        "\033[1;41m"
    };
    static constexpr CubeColorData orange = {
        "orange",
        std::array{255, 125, 7, 255},  // #FF7D07
        "\033[48;5;208m"
    };
    static constexpr CubeColorData green = {
        "green",
        std::array{40, 197, 6, 255},  // #28C506
        "\033[1;42m"
    };
    static constexpr CubeColorData blue = {
        "blue", 
        std::array{20, 138, 230, 255},  // #148AE6
        "\033[1;44m"
    };

    // Misc colors

    static constexpr CubeColorData gray = {
        "gray",
        std::array{90, 90, 90, 255},  // #5A5A5A
        "\033[48;5;250m"
    };
    static constexpr CubeColorData black = {
        "black",
        std::array{0, 0, 0, 255},  // #000000
        "\033[1;40m"
    };

    // An array of allowed cube colors (WYROGB), excluding black and gray.
    static constexpr std::array<CubeColorData, 6> allowed_cube_colors = {
        white, yellow, red, orange, green, blue
    };

    struct OppositeColorPair {
        std::string_view first;
        std::string_view second;
    };
    static constexpr std::array<OppositeColorPair, 3> opposite_pairs = {{
        {"red", "orange"}, {"yellow", "white"}, {"blue", "green"}
    }};

    // Convert a given RGBA array to ANSI color string. Only R, G, B channels are used.
    static const std::string ansi_color(ColorArray arr) {
        return std::format("\033[48;2;{};{};{}", arr[0], arr[1], arr[2]);
    };

    // Convert RGBA number array to 6-digits RGB HEX code
    // (only keep R, G, B; channel A dropped).
    static const std::string rgbToHex(ColorArray arr) {
        return std::format("#{:02X}{:02X}{:02X}", arr[0], arr[1], arr[2]);
    };

    static const CubeColorData& getColor(std::string_view color_name) {
        auto it = std::find_if(allowed_cube_colors.begin(), allowed_cube_colors.end(),
            [&](const auto& item) { return item.name == color_name; });
        if (it != allowed_cube_colors.end()) {
            return *it;
        }
        throw std::invalid_argument(
            std::format("Unknown color name: '{}'", color_name)
        );
    }
    
    static bool isOppositeColorNames(std::string_view color1, std::string_view color2) {
        for (const auto& pair : opposite_pairs) {
            if ((color1 == pair.first && color2 == pair.second ) ||
                (color1 == pair.second && color2 == pair.first )) {
                return true;
            }
        }
        return false;
    }

    static const CubeColorData& getOppositeColor(std::string_view color_name) {
        for (const auto& pair : opposite_pairs) {
            if (color_name == pair.first) return getColor(pair.second);
            if (color_name == pair.second) return getColor(pair.first);
        }
        throw std::invalid_argument(
            std::format("Cannot find the opposite of color '{}'", color_name)
        );
    }

    // Singleton class: Visit through instance().
    static const CubeColor& instance() {
        static CubeColor instance_;
        return instance_;
    }
    // No copy operation allowed.
    CubeColor(const CubeColor&) = delete;
    CubeColor& operator=(const CubeColor&) = delete;

private:
    CubeColor() = default;
};

}

#endif  // CUBIMG_COLOR_H