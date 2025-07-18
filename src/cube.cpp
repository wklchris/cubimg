#include "cube.hpp"
#include <iostream>

namespace cubimg::Cube {

// Explicit template instantiation
template class cubimg::Cube::Cube<2>;
template class cubimg::Cube::Cube<3>;
template class cubimg::Cube::Cube<4>;
template class cubimg::Cube::Cube<5>;
template class cubimg::Cube::Cube<6>;
template class cubimg::Cube::Cube<7>;


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
void Cube<N>::rotateFace90Clock(FaceGrid& face_arr) {
    const FaceGrid original_arr = face_arr;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[col][N - row - 1] = original_arr[row][col];
        }
    }
}

template<size_t N>
void Cube<N>::rotateFace90CounterClock(FaceGrid& face_arr) {
    const FaceGrid original_arr = face_arr;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[N - row - 1][col] = original_arr[row][col];
        }
    }
}

template<size_t N>
void Cube<N>::rotateFace180(FaceGrid& face_arr) {
    const FaceGrid original_arr = face_arr;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            face_arr[N - 1 - row][N - 1 - col] = original_arr[row][col];
        }
    }
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
    for (size_t row = N; row-- > 0; ) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Down)][row][col];
            std::cout << colors.at(block_color).name[0] << ' ';
        }
        std::cout << '\n';
    }
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

    // Print Down face (indented), in a reversing order
    for (size_t row = N; row-- > 0; ) {
        std::cout << std::string(face_indent, ' ');
        for (size_t col = 0; col < N; ++col) {
            Face block_color = state[static_cast<size_t>(Face::Down)][row][col];
            std::cout << colors.at(block_color).ansi << '_' << ansi_reset << ' ';
        }
        std::cout << '\n';
    }
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