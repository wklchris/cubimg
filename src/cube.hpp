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
    - row & col: both of size N. The indexing order is shown below.

    Visualization of row & col indexing of each face in net view:
    
    ```
    / / / / / x 1 2 3 
    / / / / / 1 U U U
    / / / / / 2 U U U
    / / / / / 3 U U U
    / / / / / / / / /
    x 1 2 3 - x 1 2 3 - x 1 2 3 - x 1 2 3
    1 L L L - 1 F F F - 1 R R R - 1 B B B
    2 L L L - 2 F F F - 2 R R R - 2 B B B
    3 L L L - 3 F F F - 3 R R R - 3 B B B
    / / / / / / / / /
    / / / / / x 1 2 3
    / / / / / 1 D D D
    / / / / / 2 D D D
    / / / / / 3 D D D
    ```

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