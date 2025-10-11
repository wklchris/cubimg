#ifndef CUBIMG_CFOP_H
#define CUBIMG_CFOP_H

#include "algoset.hpp"

namespace cubimg::ALGOSET {
 
constexpr std::array pll_algos = {
    Algo{"PLL-Ua", "M2' U M U2 M' U M2'"},
    Algo{"PLL-Ub", "M2' U' M U2 M' U' M2"},
    Algo{"PLL-H", "M2' U M2' U2 M2' U M2'"},
    Algo{"PLL-Z", "(M2' U M2' U) (M' U2 M2' U2) M' [U2]"},
    Algo{"PLL-Aa", "x' R2 D2' (R' U' R) D2' (R' U R')", "x'"},
    Algo{"PLL-Ab", "x' (R U' R) D2 (R' U R) D2 R2'", "x'"},
    Algo{"PLL-E", "x' (R U' R' D) (R U R' D') (R U R' D) (R U' R' D')", "x'"},
    Algo{"PLL-T", "(R U R' U') R' F (R2 U' R' U') (R U R' F')"},
    Algo{"PLL-F", "(R' U' F') (R U R' U') R' F (R2 U' R' U') (R U R' U R)"},
    Algo{"PLL-V", "(R' U R' U') (R D' R' D) (R' U D') R2 U' R2 D R2"},
    Algo{"PLL-Y", "F (R U' R' U') (R U R' F') (R U R' U') (R' F R F')"},
    Algo{"PLL-Ja", "(l2' U) (l F' R) U2' (r' U r) U2'", "x"},
    Algo{"PLL-Jb", "(R U R' F') (R U R' U') R' F R2 U' R' [U']"},
    Algo{"PLL-Rb", "(R' U2 R U2') R' F (R U R' U') R' F' R2 [U'])"},
    Algo{"PLL-Ra", "(R U' R' U') (R U R D) (R' U' R D') R' U2 R' U"},
    Algo{"PLL-Gc", "(R2' U' R U') (R U R' U) R2 (U D') (R U' R') D [U']"},
    Algo{"PLL-Gd", "(R U R' U' D) (R2 U' R U') (R' U R' U R2) D' [U]"},
    Algo{"PLL-Ga", "(R2 U R' U) (R' U' R U') R2 U' D (R' U R) D' [U]"},
    Algo{"PLL-Gb", "(R' U' R U D') R2 U (R' U R U' R U') R2 D"},
    Algo{"PLL-Nb", "(R' U R U') (R' F' U' F) (R U R' F) (R' F' R U' R)"},
    Algo{"PLL-Na", "(R U R' U) (R U R' F') (R U R' U') R' F (R2 U' R') U2 (R U' R')"}
};

}

#endif  // CUBIMG_CFOP_H