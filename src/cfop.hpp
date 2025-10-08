#ifndef CUBIMG_CFOP_H
#define CUBIMG_CFOP_H

#include <string_view>
#include <array>

namespace cubimg::CFOP {

struct SetupAlgo {
    std::string_view name;
    std::string_view setup;
};

constexpr std::array pll_setup = {
    SetupAlgo{"PLL-Ua", "M2' U' M U2 M' U' M2"},
    SetupAlgo{"PLL-Ub", "M2' U M U2 M' U M2'"},
    SetupAlgo{"PLL-H", "M2' U M2' U2 M2' U M2'"},
    SetupAlgo{"PLL-Z", "M2' U M2' U M' U2 M2' U2 M' U2"},
    SetupAlgo{"PLL-Aa", "x' R U' R D2 R' U R D2 R2 x"},
    SetupAlgo{"PLL-Ab", "x' R2 D2 R' U' R D2 R' U R' x"},
    SetupAlgo{"PLL-E", "x' R U' R' D R U R' D' R U R' D R U' R' D' x"},
    SetupAlgo{"PLL-T", "R U R' U' R' F R2 U' R' U' R U R' F' U'"},
    SetupAlgo{"PLL-F", "R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R"},
    SetupAlgo{"PLL-V", "R' U R' U' R D' R' D R' U D' R2 U' R2 D R2"},
    SetupAlgo{"PLL-Y", "F R U' R' U' R U R' F' R U R' U' R' F R F'"},
    SetupAlgo{"PLL-Ja", "L' U R' z R2 U R' U' R2 U D R' z'"},
    SetupAlgo{"PLL-Jb", "R U R' F' R U R' U' R' F R2 U' R' U'"},
    SetupAlgo{"PLL-Rb", "R' U2 R U2' R' F R U R' U' R' F' R2 U'"},
    SetupAlgo{"PLL-Ra", "R U' R' U' R U R D R' U' R D' R' U2 R' U2"},
    SetupAlgo{"PLL-Gc", "R U R' U' D R2 U' R U' R' U R' U R2 D' U'"},
    SetupAlgo{"PLL-Gd", "R2' U' R U' R U R' U R2 U D' R U' R' D U'"},
    SetupAlgo{"PLL-Ga", "R' U' R U D' R2 U R' U R U' R U' R2 D U'"},
    SetupAlgo{"PLL-Gb", "R2 U R' U R' U' R U' R2 U' D R' U R D' U'"},
    SetupAlgo{"PLL-Nb", "R' U R U' R' F' U' F R U R' U' R U' f R f'"},
    SetupAlgo{"PLL-Na", "R U R' U R U R' F' R U R' U' R' F R2 U' R' U2 R U' R'"}
};

}

#endif  // CUBIMG_CFOP_H