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
    Clock, CounterClock,
    HalfTurn, HalfTurnReversed,
    TripleQuarters, TripleQuartersReversed 
};

// Reverse a cube rotation (e.g., R to R')
inline Rotation reverseRotation(Rotation rotation) {
    using enum Rotation;
    switch (rotation) {
        case Clock: return CounterClock;
        case CounterClock: return Clock;
        case HalfTurn: return HalfTurnReversed;
        case HalfTurnReversed: return HalfTurn;
        case TripleQuarters: return TripleQuartersReversed;
        case TripleQuartersReversed: return TripleQuarters;
        default: return static_cast<Rotation>(-1);
    }
}

struct AlgoStep {
    // A face character is either: U,L,F,R,B,D,x,y,z,M,S,E
    char face = '!';
    size_t layers = 1;
    Rotation rotation = Rotation::Clock;
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

    // Rotation the M layer. Only available for 3-order cubes
    void rotateM(Rotation rotation);
    // Rotation the S layer. Only available for 3-order cubes
    void rotateS(Rotation rotation);
    // Rotation the E layer. Only available for 3-order cubes
    void rotateE(Rotation rotation);

    // Rotate full cubes

    void rotateX(Rotation rotation);
    void rotateY(Rotation rotation);
    void rotateZ(Rotation rotation);

    // Parse single algo token to AlgoStep.
    AlgoStep parseStepFromToken(std::string_view token);
    void rotateStep(AlgoStep step);
    
    // Apply a rotation algo (e.g., R) to the cube.
    void applyAlgoToken(std::string_view token);
    // Apply a reversed algo (e.g., apply R' when given R) to the cube.
    void applyAlgoTokenReversed(std::string_view token);

public:
    Cube();
    virtual ~Cube() = default;

    size_t order() const { return N; };
    
    // Single whole cube rotations

    void x() { rotateX(Rotation::Clock); }
    void xprime() { rotateX(Rotation::CounterClock); }
    void x2() { rotateX(Rotation::HalfTurn); }

    void y() { rotateY(Rotation::Clock); }
    void yprime() { rotateY(Rotation::CounterClock); }
    void y2() { rotateY(Rotation::HalfTurn); }

    void z() { rotateZ(Rotation::Clock); }
    void zprime() { rotateZ(Rotation::CounterClock); }
    void z2() { rotateZ(Rotation::HalfTurn); }

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

    // Single order-3 cube moves

    void M() { rotateM(Rotation::Clock); };
    void Mprime() { rotateM(Rotation::CounterClock); };
    void M2() { rotateM(Rotation::HalfTurn); };

    void S() { rotateS(Rotation::Clock); };
    void Sprime() { rotateS(Rotation::CounterClock); };
    void S2() { rotateS(Rotation::HalfTurn); };

    void E() { rotateE(Rotation::Clock); };
    void Eprime() { rotateE(Rotation::CounterClock); };
    void E2() { rotateE(Rotation::HalfTurn); };

    // Rotate the cube according to the given algorithm steps.
    void applyAlgo(std::string_view algo);
    // Rotate the cube according to the reverse of a given algorithm string.
    // The "reverse" means both: (1) the order of rotation; (2) the rotation angle.
    // E.g., the algo "R U F" will be reversed to "F' U' R'".
    void applyAlgoReverse(std::string_view algo);
    
    // Return RGBA of the (default) color for a face
    std::array<int, 4> getFaceColorRGBA(Face f) const;
    // Return HEX code of the (default) color for a face
    std::string getFaceColorHex(Face f) const;
    // Return the color array of all blocks for a specific face
    std::array<std::array<Face, N>, N> getFaceColorArray(Face f) const;

    // Print the cube state in pure text.
    // Each block is printed with color initials (e.g. 'r' for red).
    void printCubeText() const;
    // Print the cube state as colored string, using ANSI color code.
    void printCubeInColor() const;

    void resetCubeState();
};

}

#endif  // CUBIMG_CUBE_H