#ifndef CUBIMG_CUBE_H
#define CUBIMG_CUBE_H

#include <cstddef>
#include <algorithm>
#include <array>
#include <iostream>
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

// Cube class for cubes of 2~7 order.
template<size_t N>
class Cube {
    static_assert(N >= 2 && N <= 7, "Required: 2 <= N <= 7");

    using FaceArray = std::array<std::array<Face, N>, N>;
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

    // Helper function to rotate a face (not a layer) by 90 degree clockwise
    void rotateFace90Clock_(Face face);
    // Helper function to rotate a face (not a layer) by 90 degree counter-clockwise
    void rotateFace90CounterClock_(Face face);
    // Helper function to rotate a face (not a layer) by 180 degree
    void rotateFace180_(Face face);

    // Rotate multiple layers with given rotation angle.
    // e.g. rotateRw(2, Rotation::Clock) -> r (or say: 2Rw)

    void rotateRw(Rotation rotation, size_t layers = 1);
    void rotateLw(Rotation rotation, size_t layers = 1);
    void rotateUw(Rotation rotation, size_t layers = 1);
    void rotateDw(Rotation rotation, size_t layers = 1);
    void rotateFw(Rotation rotation, size_t layers = 1);
    void rotateBw(Rotation rotation, size_t layers = 1);

public:
    Cube();
    virtual ~Cube() = default;

    size_t order() const { return N; };
    
    // Single cube moves

    void R() { rotateRw(Rotation::Clock); };
    void Rprime() { rotateRw(Rotation::CounterClock); };
    void R2() { rotateRw(Rotation::HalfTurn); };
    void r() { rotateRw(Rotation::Clock, N-1); };
    void rprime() { rotateRw(Rotation::CounterClock, N-1); };
    void r2() { rotateRw(Rotation::HalfTurn, N-1); };

    void L() { rotateLw(Rotation::Clock); };
    void Lprime() { rotateLw(Rotation::CounterClock); };
    void L2() { rotateLw(Rotation::HalfTurn); };
    void l() { rotateLw(Rotation::Clock, N-1); };
    void lprime() { rotateLw(Rotation::CounterClock, N-1); };
    void l2() { rotateLw(Rotation::HalfTurn, N-1); };

    void U() { rotateUw(Rotation::Clock); };
    void Uprime() { rotateUw(Rotation::CounterClock); };
    void U2() { rotateUw(Rotation::HalfTurn); };
    void u() { rotateUw(Rotation::Clock, N-1); };
    void uprime() { rotateUw(Rotation::CounterClock, N-1); };
    void u2() { rotateUw(Rotation::HalfTurn, N-1); };

    void D() { rotateDw(Rotation::Clock); };
    void Dprime() { rotateDw(Rotation::CounterClock); };
    void D2() { rotateDw(Rotation::HalfTurn); };
    void d() { rotateDw(Rotation::Clock, N-1); };
    void dprime() { rotateDw(Rotation::CounterClock, N-1); };
    void d2() { rotateDw(Rotation::HalfTurn, N-1); };

    void F() { rotateFw(Rotation::Clock); };
    void Fprime() { rotateFw(Rotation::CounterClock); };
    void F2() { rotateFw(Rotation::HalfTurn); };
    void f() { rotateFw(Rotation::Clock, N-1); };
    void fprime() { rotateFw(Rotation::CounterClock, N-1); };
    void f2() { rotateFw(Rotation::HalfTurn, N-1); };

    void B() { rotateBw(Rotation::Clock); };
    void Bprime() { rotateBw(Rotation::CounterClock); };
    void B2() { rotateBw(Rotation::HalfTurn); };
    void b() { rotateBw(Rotation::Clock, N-1); };
    void bprime() { rotateBw(Rotation::CounterClock, N-1); };
    void b2() { rotateBw(Rotation::HalfTurn, N-1); };

    // Print the cube state in pure text.
    // Each block is printed with color initials (e.g. 'r' for red).
    void printCubeText() const;
    // Print the cube state as colored string, using ANSI color code.
    void printCubeInColor() const;

    void resetCubeState();
};

}

#endif  // CUBIMG_CUBE_H