#ifndef CUBIMG_COLOR_H
#define CUBIMG_COLOR_H

#include <array>
#include <format>

namespace cubimg::Color {

struct CubeColorData {
    using ColorArray = std::array<int, 4>;

    std::string_view name;
    ColorArray rgba;
    std::string_view ansi;
};

class CubeColor {
    using ColorArray = std::array<int, 4>;

public:
    static constexpr std::string_view ansi_reset = "\033[0m";

    // Face colors

    inline static constexpr CubeColorData white = {
        "white", 
        std::array{255, 255, 255, 255},  // #FFFFFF
        "\033[1;47m"
    };
    inline static constexpr CubeColorData yellow = {
        "yellow",
        std::array{238, 242, 12, 255},  // #EEF20C
        "\033[48;5;226m"
    };
    inline static constexpr CubeColorData red = {
        "red",
        std::array{243, 41, 41, 255},  // #F32929
        "\033[1;41m"
    };
    inline static constexpr CubeColorData orange = {
        "orange",
        std::array{255, 125, 7, 255},  // #FF7D07
        "\033[48;5;208m"
    };
    inline static constexpr CubeColorData green = {
        "green",
        std::array{40, 197, 6, 255},  // #28C506
        "\033[1;42m"
    };
    inline static constexpr CubeColorData blue = {
        "blue", 
        std::array{20, 138, 230, 255},  // #148AE6
        "\033[1;44m"
    };

    // Misc colors

    inline static constexpr CubeColorData gray = {
        "gray",
        std::array{90, 90, 90, 255},
        "\033[48;5;250m"
    };
    inline static constexpr CubeColorData black = {
        "black",
        std::array{0, 0, 0, 255},
        "\033[1;40m"
    };

    static const std::string ansi_color(ColorArray arr) {
        return std::format("\033[48;2;{};{};{}", arr[0], arr[1], arr[2]);
    };

    // Convert RGBA number array to 6-digits RGB HEX code
    // (only keep R, G, B; channel A dropped).
    static const std::string rgbToHex(ColorArray arr) {
        return std::format("#{:02X}{:02X}{:02X}", arr[0], arr[1], arr[2]);
    };

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