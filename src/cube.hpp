#ifndef CUBIMG_CUBE_H
#define CUBIMG_CUBE_H

#include <cstddef>
#include <array>
#include <string>
#include <unordered_map>
#include "color.hpp"

namespace cubimg::Cube {

enum class Face {
    Up, Down, Front, Back, Left, Right
};

inline std::string faceName(Face face_) {
    static constexpr std::array<std::string, 6> face_names = {
        "U", "D", "F", "B", "L", "R"
    };
    return face_names[static_cast<size_t>(face_)];
}


enum class Rotation {
    Clock, CounterClock, HalfTurn
};


template<size_t N>
class Cube {
    using FaceGrid = std::array<std::array<Face, N>, N>;
    /*
    Cube state stored using Faces, representing the face's default color.
    It is a 3D array of [face][row][col].
    - face: size 6, in order of U, D, F, B, L, R.
    - row: size N, from bottom to top of a cube face.
    - col: size N, from left to right of a cube face.
    */ 
    using CubeState = std::array<std::array<std::array<Face, N>, N>, 6>;
private: 
    // The current state of the cube.
    CubeState state;
    // Cube colors for each face.
    std::unordered_map<Face, cubimg::Color::CubeColorData> colors;

    void rotateFace90Clock(FaceGrid& face_arr);
    void rotateFace90CounterClock(FaceGrid& face_arr);
    void rotateFace180(FaceGrid& face_arr);

public:
    Cube();
    virtual ~Cube() = default;

    size_t order() const { return N; };
    
    // Print the cube state in pure text.
    // Each block is printed with color initials (e.g. 'r' for red).
    void printCubeText() const;
    // Print the cube state as colored string, using ANSI color code.
    void printCubeInColor() const;

    void resetCubeState();
};

}

#endif  // CUBIMG_CUBE_H