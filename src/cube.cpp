#include "cube.hpp"
#include "color.hpp"
#include <cctype>
#include <iostream>
#include <vector>

namespace cubimg::Cube {

// Explicit template instantiation

template class Cube<2>;
template class Cube<3>;
template class Cube<4>;
template class Cube<5>;
template class Cube<6>;
template class Cube<7>;


// Template Class Definitions

template<size_t N>
Cube<N>::Cube() {
    resetCubeState();

    const auto& CubeColor = cubimg::Color::CubeColor::instance();
    colors = {
        {Face::Up,    CubeColor.white},
        {Face::Down,  CubeColor.yellow},
        {Face::Front, CubeColor.green},
        {Face::Back,  CubeColor.blue},
        {Face::Left,  CubeColor.orange},
        {Face::Right, CubeColor.red}
    };
}

template<size_t N>
void Cube<N>::rotateFace90Clock_(Face face) {
    const FaceArray original_arr = state[static_cast<size_t>(face)];
    FaceArray& face_arr = state[static_cast<size_t>(face)];
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[col][N - row - 1] = original_arr[row][col];
        }
    }
}

template<size_t N>
void Cube<N>::rotateFace90CounterClock_(Face face) {
    const FaceArray original_arr = state[static_cast<size_t>(face)];
    FaceArray& face_arr = state[static_cast<size_t>(face)];
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[N - col - 1][row] = original_arr[row][col];
        }
    }
}

template<size_t N>
void Cube<N>::rotateFace180_(Face face) {
    const FaceArray original_arr = state[static_cast<size_t>(face)];
    FaceArray& face_arr = state[static_cast<size_t>(face)];
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[N - 1 - row][N - 1 - col] = original_arr[row][col];
        }
    }
}

template<size_t N>
void Cube<N>::rotateRw(Rotation rotation, size_t layers) {
    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];

    switch(rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Right);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Left);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = N - layers; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = F_arr[row][col];
                    F_arr[row][col] = D_arr[row][col];
                    D_arr[row][col] = B_arr[N-1-row][N-1-col];
                    B_arr[N-1-row][N-1-col] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Right);
            if (layers == N) {
                rotateFace90Clock_(Face::Left);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = N - layers; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = B_arr[N-1-row][N-1-col];
                    B_arr[N-1-row][N-1-col] = D_arr[row][col];
                    D_arr[row][col] = F_arr[row][col];
                    F_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Right);
            if (layers == N) {
                rotateFace180_(Face::Left);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = N - layers; col < N; ++col) {
                    std::swap(U_arr[row][col], D_arr[row][col]);
                    std::swap(F_arr[row][col], B_arr[N-1-row][N-1-col]);
                }
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateLw(Rotation rotation, size_t layers) {   
    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];

    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Left);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Right);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = 0; col < layers; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = B_arr[N-1-row][N-1-col];
                    B_arr[N-1-row][N-1-col] = D_arr[row][col];
                    D_arr[row][col] = F_arr[row][col];
                    F_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Left);
            if (layers == N) {
                rotateFace90Clock_(Face::Right);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = 0; col < layers; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = F_arr[row][col];
                    F_arr[row][col] = D_arr[row][col];
                    D_arr[row][col] = B_arr[N-1-row][N-1-col];
                    B_arr[N-1-row][N-1-col] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Left);
            if (layers == N) {
                rotateFace180_(Face::Right);
            }
            for (size_t row = 0; row < N; ++row) {
                for (size_t col = 0; col < layers; ++col) {
                    std::swap(U_arr[row][col], D_arr[row][col]);
                    std::swap(F_arr[row][col], B_arr[N-1-row][N-1-col]);
                }
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateUw(Rotation rotation, size_t layers) {    
    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Up);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Down);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = F_arr[row][col];
                    F_arr[row][col] = R_arr[row][col];
                    R_arr[row][col] = B_arr[row][col];
                    B_arr[row][col] = L_arr[row][col];
                    L_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Up);
            if (layers == N) {
                rotateFace90Clock_(Face::Down);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = F_arr[row][col];
                    F_arr[row][col] = L_arr[row][col];
                    L_arr[row][col] = B_arr[row][col];
                    B_arr[row][col] = R_arr[row][col];
                    R_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Up);
            if (layers == N) {
                rotateFace180_(Face::Down);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    std::swap(F_arr[row][col], B_arr[row][col]);
                    std::swap(L_arr[row][col], R_arr[row][col]);
                }
            }
            break;
        default:
            break;
    }

    
}

template<size_t N>
void Cube<N>::rotateDw(Rotation rotation, size_t layers) {   
    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Down);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Up);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = F_arr[row][col];
                    F_arr[row][col] = L_arr[row][col];
                    L_arr[row][col] = B_arr[row][col];
                    B_arr[row][col] = R_arr[row][col];
                    R_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Down);
            if (layers == N) {
                rotateFace90Clock_(Face::Up);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = F_arr[row][col];
                    F_arr[row][col] = R_arr[row][col];
                    R_arr[row][col] = B_arr[row][col];
                    B_arr[row][col] = L_arr[row][col];
                    L_arr[row][col] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Down);
            if (layers == N) {
                rotateFace180_(Face::Up);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    std::swap(F_arr[row][col], B_arr[row][col]);
                    std::swap(L_arr[row][col], R_arr[row][col]);
                }
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateFw(Rotation rotation, size_t layers) {    
    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Front);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Back);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = L_arr[N-1-col][row];
                    L_arr[N-1-col][row] = D_arr[N-1-row][N-1-col];
                    D_arr[N-1-row][N-1-col] = R_arr[col][N-1-row];
                    R_arr[col][N-1-row] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Front);
            if (layers == N) {
                rotateFace90Clock_(Face::Back);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = R_arr[col][N-1-row];
                    R_arr[col][N-1-row] = D_arr[N-1-row][N-1-col];
                    D_arr[N-1-row][N-1-col] = L_arr[N-1-col][row];
                    L_arr[N-1-col][row] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Front);
            if (layers == N) {
                rotateFace180_(Face::Back);
            }
            for (size_t row = N - layers; row < N; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    std::swap(U_arr[row][col], D_arr[N-1-row][N-1-col]);
                    std::swap(L_arr[N-1-col][row], R_arr[col][N-1-row]);
                }
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateBw(Rotation rotation, size_t layers) {    
    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            rotateFace90Clock_(Face::Back);
            if (layers == N) {
                rotateFace90CounterClock_(Face::Front);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = R_arr[col][N-1-row];
                    R_arr[col][N-1-row] = D_arr[N-1-row][N-1-col];
                    D_arr[N-1-row][N-1-col] = L_arr[N-1-col][row];
                    L_arr[N-1-col][row] = temp_face;
                }
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            rotateFace90CounterClock_(Face::Back);
            if (layers == N) {
                rotateFace90Clock_(Face::Front);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    const Face temp_face = U_arr[row][col];
                    U_arr[row][col] = L_arr[N-1-col][row];
                    L_arr[N-1-col][row] = D_arr[N-1-row][N-1-col];
                    D_arr[N-1-row][N-1-col] = R_arr[col][N-1-row];
                    R_arr[col][N-1-row] = temp_face;
                }
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            rotateFace180_(Face::Back);
            if (layers == N) {
                rotateFace180_(Face::Front);
            }
            for (size_t row = 0; row < layers; ++row) {
                for (size_t col = 0; col < N; ++col) {
                    std::swap(U_arr[row][col], D_arr[N-1-row][N-1-col]);
                    std::swap(L_arr[N-1-col][row], R_arr[col][N-1-row]);
                }
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateM(Rotation rotation) {
    if constexpr (N != 3) {
        std::cerr << "M rotations is only available for order-3 cubes.";
        return;
    }

    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];

    // Rotate middle layer M, same direction as L.
    const size_t col = 1;
    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            for (size_t row = 0; row < N; ++row) {
                const Face temp_face = U_arr[row][col];
                U_arr[row][col] = B_arr[N-1-row][N-1-col];
                B_arr[N-1-row][N-1-col] = D_arr[row][col];
                D_arr[row][col] = F_arr[row][col];
                F_arr[row][col] = temp_face;
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            for (size_t row = 0; row < N; ++row) {
                const Face temp_face = U_arr[row][col];
                U_arr[row][col] = F_arr[row][col];
                F_arr[row][col] = D_arr[row][col];
                D_arr[row][col] = B_arr[N-1-row][N-1-col];
                B_arr[N-1-row][N-1-col] = temp_face;
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            for (size_t row = 0; row < N; ++row) {
                std::swap(U_arr[row][col], D_arr[row][col]);
                std::swap(F_arr[row][col], B_arr[N-1-row][N-1-col]);
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateS(Rotation rotation) {
    if constexpr (N != 3) {
        std::cerr << "S rotations is only available for order-3 cubes.";
        return;
    }

    FaceArray& U_arr = state[static_cast<size_t>(Face::Up)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& D_arr = state[static_cast<size_t>(Face::Down)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    // Rotate middle layer S, same direction as F.
    const size_t row = 1;
    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            for (size_t col = 0; col < N; ++col) {
                const Face temp_face = U_arr[row][col];
                U_arr[row][col] = L_arr[N-1-col][row];
                L_arr[N-1-col][row] = D_arr[N-1-row][N-1-col];
                D_arr[N-1-row][N-1-col] = R_arr[col][N-1-row];
                R_arr[col][N-1-row] = temp_face;
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            for (size_t col = 0; col < N; ++col) {
                const Face temp_face = U_arr[row][col];
                U_arr[row][col] = R_arr[col][N-1-row];
                R_arr[col][N-1-row] = D_arr[N-1-row][N-1-col];
                D_arr[N-1-row][N-1-col] = L_arr[N-1-col][row];
                L_arr[N-1-col][row] = temp_face;
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            for (size_t col = 0; col < N; ++col) {
                std::swap(U_arr[row][col], D_arr[N-1-row][N-1-col]);
                std::swap(L_arr[N-1-col][row], R_arr[col][N-1-row]);
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateE(Rotation rotation) {
    if constexpr (N != 3) {
        std::cerr << "S rotations is only available for order-3 cubes.";
        return;
    }

    FaceArray& F_arr = state[static_cast<size_t>(Face::Front)];
    FaceArray& L_arr = state[static_cast<size_t>(Face::Left)];
    FaceArray& B_arr = state[static_cast<size_t>(Face::Back)];
    FaceArray& R_arr = state[static_cast<size_t>(Face::Right)];

    // Rotate middle layer E, same direction as D.
    const size_t row = 1;
    switch (rotation) {
        case Rotation::Clock:
        case Rotation::TripleQuartersReversed:
            for (size_t col = 0; col < N; ++col) {
                const Face temp_face = F_arr[row][col];
                F_arr[row][col] = L_arr[row][col];
                L_arr[row][col] = B_arr[row][col];
                B_arr[row][col] = R_arr[row][col];
                R_arr[row][col] = temp_face;
            }
            break;
        case Rotation::CounterClock:
        case Rotation::TripleQuarters:
            for (size_t col = 0; col < N; ++col) {
                const Face temp_face = F_arr[row][col];
                F_arr[row][col] = R_arr[row][col];
                R_arr[row][col] = B_arr[row][col];
                B_arr[row][col] = L_arr[row][col];
                L_arr[row][col] = temp_face;
            }
            break;
        case Rotation::HalfTurn:
        case Rotation::HalfTurnReversed:
            for (size_t col = 0; col < N; ++col) {
                std::swap(F_arr[row][col], B_arr[row][col]);
                std::swap(L_arr[row][col], R_arr[row][col]);
            }
            break;
        default:
            break;
    }
}

template<size_t N>
void Cube<N>::rotateX(Rotation rotation) {
    rotateRw(rotation, N);
}

template<size_t N>
void Cube<N>::rotateY(Rotation rotation) {
    rotateUw(rotation, N);
}

template<size_t N>
void Cube<N>::rotateZ(Rotation rotation) {
    rotateFw(rotation, N);
}

template<size_t N>
void Cube<N>::rotateStep(AlgoStep step) {
    auto [face, layers, rotation] = step;
    switch (face) {
        case 'R': rotateRw(rotation, layers); break;
        case 'L': rotateLw(rotation, layers); break;
        case 'U': rotateUw(rotation, layers); break;
        case 'D': rotateDw(rotation, layers); break;
        case 'F': rotateFw(rotation, layers); break;
        case 'B': rotateBw(rotation, layers); break;
        case 'M': rotateM(rotation); break;
        case 'S': rotateS(rotation); break;
        case 'E': rotateE(rotation); break;
        case 'x': rotateX(rotation); break;
        case 'y': rotateY(rotation); break;
        case 'z': rotateZ(rotation); break;
        default:
            break;
    }
}

template<size_t N>
AlgoStep Cube<N>::parseStepFromToken(std::string_view token) {
    if (token.empty()) return AlgoStep{};

    // Parse the number of layers to rotate, e.g. 2Rw
    size_t layers = 1;
    size_t pos = 0;
    while (pos < token.size() && std::isdigit(token[pos])) {
        pos++;
    }
    if (pos > 0) {
        layers = std::stoi(std::string(token.substr(0, pos)));
    }

    if (pos >= token.size()) {
        return AlgoStep{};
    }
    char face = token[pos];
    Rotation rotation = Rotation::Clock;

    // Check if the rotation note is a wide rotation (e.g. Rw)
    // or a lowercase, non-xyz notation (e.g. r2)
    if ((pos + 1 < token.size() && token[pos + 1] == 'w') || 
        std::islower(face)) {
        if (face == 'x' || face == 'y' || face == 'z') {
            layers = N;
        } else if (std::islower(face)) {
            face = std::toupper(face);
            layers = N - 1;
        } else {
            // A wide rotation rotates 2 layers by default.
            layers = layers > 2 ? layers : 2;
            pos++;  // Skip 'w'
        }
    }

    // Parse rotation direction
    if (pos + 1 < token.size()) {
        if (token[pos + 1] == '\'') {
            if (pos + 2 < token.size()) {
                switch (token[pos + 2]) {
                    case '2':  // Example: R'2
                        rotation = Rotation::HalfTurnReversed;
                        pos += 3;
                        break;
                    case '3':  // Example: R'3
                        rotation = Rotation::TripleQuartersReversed;
                        pos += 3;
                        break;
                    default:  // Example: R'
                        rotation = Rotation::CounterClock;
                        pos += 2;
                }
            } else {  // If at end of token
                rotation = Rotation::CounterClock;
                pos += 2;
            }
        } else if (token[pos + 1] == '2') {  // Example: R2
            rotation = Rotation::HalfTurn;
            pos += 2;
        } else if (token[pos + 1] == '3') {  // Example: R3
            rotation = Rotation::TripleQuarters;
            pos += 2;
        } else {
            pos++;
        }
    }
    return AlgoStep{face, layers, rotation};
}

template<size_t N>
void Cube<N>::applyAlgoToken(std::string_view token) {
    AlgoStep algo_step = parseStepFromToken(token);
    rotateStep(algo_step);
}

template<size_t N>
void Cube<N>::applyAlgoTokenReversed(std::string_view token) {
    AlgoStep algo_step = parseStepFromToken(token);
    algo_step.rotation = reverseRotation(algo_step.rotation);
    rotateStep(algo_step);
}

template<size_t N>
void Cube<N>::applyAlgo(std::string_view algo) {
    size_t start = 0;
    size_t end = algo.find(' ');

    while (start < algo.size()) {
        // Skip spaces
        while (start < algo.size() && std::isspace(algo[start])) {
            start++;
        }
        if (start >= algo.size()) break;

        // Skip round & square brackets
        if (algo[start] == '(' || algo[start] == ')' || 
            algo[start] == '[' || algo[start] == ']') {
            start++;
            continue;
        }

        // Find the current token end
        end = algo.find(' ', start);
        if (end == std::string_view::npos) {
            end = algo.size();
        }

        // Apply the token rotation
        std::string_view token = algo.substr(start, end - start);
        if (!token.empty()) {
            applyAlgoToken(token);
        }

        start = end + 1;
    }
}

template<size_t N>
void Cube<N>::applyAlgoReverse(std::string_view algo) {
    // This function has the exact same logic with function applyAlgo(),
    // except that it iters from right and calls applyAlgoTokenReverse(). 

    std::vector<std::string_view> tokens;
    size_t start = 0;
    size_t end = algo.find(' ');

    while (start < algo.size()) {
        // Skip spaces
        while (start < algo.size() && std::isspace(algo[start])) {
            start++;
        }
        if (start >= algo.size()) break;

        // Skip round & square brackets
        if (algo[start] == '(' || algo[start] == ')' || 
            algo[start] == '[' || algo[start] == ']') {
            start++;
            continue;
        }

        // Find the current token end
        end = algo.find(' ', start);
        if (end == std::string_view::npos) {
            end = algo.size();
        }

        // Apply the token rotation
        std::string_view token = algo.substr(start, end - start);
        if (!token.empty()) {
            tokens.push_back(token);
        }

        start = end + 1;
    }

    // Iterate the token vector in a reversed order and apply reversed rotation
    for (auto it = tokens.rbegin(); it != tokens.rend(); ++it) {
        applyAlgoTokenReversed(*it);
    }
}

template<size_t N>
std::array<int, 4> Cube<N>::getFaceColorRGBA(Face f) const {
    return colors.at(f).rgba;
}

template<size_t N>
std::string Cube<N>::getFaceColorHex(Face f) const {
    auto _rgba = colors.at(f).rgba;
    return cubimg::Color::CubeColor::rgbToHex(_rgba);
}

template<size_t N>
std::array<std::array<Face, N>, N> Cube<N>::getFaceColorArray(Face f) const {
    return state[static_cast<size_t>(f)];
}

template<size_t N>
void Cube<N>::printCubeText() const {
    const std::string face_sep = "| ";
    const size_t face_indent = N * 2 + face_sep.length();

    // Print Up face (indented)
    for (size_t row = 0; row < N; ++row) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Up)][row][col];
            std::cout << colors.at(block_color).name[0] << ' ';
        }
        std::cout << '\n';
    }

    // Print Left, Front, Right, Back faces
    for (size_t row = 0; row < N; ++row) {
        for (Face face_ : {Face::Left, Face::Front, Face::Right, Face::Back}) {
            for (size_t col = 0; col < N; ++col) {
                Face block_color = state[static_cast<size_t>(face_)][row][col];
                std::cout << colors.at(block_color).name[0] << ' ';
            }
            std::cout << face_sep;
        }
        std::cout << '\n';
    }

    // Print Down face (indented)
    for (size_t row = 0; row < N; ++row) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Down)][row][col];
            std::cout << colors.at(block_color).name[0] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

template<size_t N>
void Cube<N>::printCubeInColor() const {
    const std::string face_sep = " ";
    const size_t face_indent = N * 2 + face_sep.length();

    const auto ansi_reset = cubimg::Color::CubeColor::ansi_reset;

    // Print Up face (indented)
    for (size_t row = 0; row < N; ++row) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Up)][row][col];
            std::cout << colors.at(block_color).ansi << '_' << ansi_reset << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Print Left, Front, Right, Back faces
    for (size_t row = 0; row < N; ++row) {
        for (Face face_ : {Face::Left, Face::Front, Face::Right, Face::Back}) {
            for (size_t col = 0; col < N; ++col) {
                Face block_color = state[static_cast<size_t>(face_)][row][col];
                std::cout << colors.at(block_color).ansi << '_' << ansi_reset << ' ';
            }
            std::cout << face_sep;
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Print Down face (indented, reversed rows)
    for (size_t row = 0; row < N; ++row) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Down)][row][col];
            std::cout << colors.at(block_color).ansi << '_' << ansi_reset << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

template<size_t N>
void Cube<N>::resetCubeState() {
    for (size_t face_index = 0; face_index < 6; ++face_index) {
        Face color = static_cast<Face>(face_index);
        for (size_t row = 0; row < N; ++row) {
            for (size_t col = 0; col < N; ++col) {
                state[face_index][row][col] = color;
            }
        }
    }
}

}