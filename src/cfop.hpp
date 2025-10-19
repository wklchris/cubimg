#ifndef CUBIMG_CFOP_H
#define CUBIMG_CFOP_H

#include "algoset.hpp"

namespace cubimg::ALGOSET {
 
constexpr std::array<Algo, 21> pll_algos = {{
    {"PLL-Ua", "M2' U M U2 M' U M2'"},
    {"PLL-Ub", "M2' U' M U2 M' U' M2"},
    {"PLL-H", "M2' U M2' U2 M2' U M2'"},
    {"PLL-Z", "(M2' U M2' U) (M' U2 M2' U2) M' [U2]"},
    {"PLL-Aa", "x' R2 D2' (R' U' R) D2' (R' U R')", "x'"},
    {"PLL-Ab", "x' (R U' R) D2 (R' U R) D2 R2'", "x'"},
    {"PLL-E", "x' (R U' R' D) (R U R' D') (R U R' D) (R U' R' D')", "x'"},
    {"PLL-T", "(R U R' U') R' F (R2 U' R' U') (R U R' F')"},
    {"PLL-F", "(R' U' F') (R U R' U') R' F (R2 U' R' U') (R U R' U R)"},
    {"PLL-V", "(R' U R' U') (R D' R' D) (R' U D') R2 U' R2 D R2"},
    {"PLL-Y", "F (R U' R' U') (R U R' F') (R U R' U') (R' F R F')"},
    {"PLL-Ja", "(l2' U) (l F' R) U2' (r' U r) U2'", "x"},
    {"PLL-Jb", "(R U R' F') (R U R' U') R' F R2 U' R' [U']"},
    {"PLL-Rb", "(R' U2 R U2') R' F (R U R' U') R' F' R2 [U'])"},
    {"PLL-Ra", "(R U' R' U') (R U R D) (R' U' R D') R' U2 R' U"},
    {"PLL-Gc", "(R2' U' R U') (R U R' U) R2 (U D') (R U' R') D [U']"},
    {"PLL-Gd", "(R U R' U' D) (R2 U' R U') (R' U R' U R2) D' [U]"},
    {"PLL-Ga", "(R2 U R' U) (R' U' R U') R2 U' D (R' U R) D' [U]"},
    {"PLL-Gb", "(R' U' R U D') R2 U (R' U R U' R U') R2 D"},
    {"PLL-Nb", "(R' U R U') (R' F' U' F) (R U R' F) (R' F' R U' R)"},
    {"PLL-Na", "(R U R' U) (R U R' F') (R U R' U') R' F (R2 U' R') U2 (R U' R')"}
}};

constexpr std::array<Algo, 57> oll_algos = {{
    // Cross (2 Look OLL) cases
    {"OLL-21", "R U (R' U R U') R' U (R U2' R')"},
    {"OLL-22", "R U2' (R2' U' R2 U') (R2' U2' R)"},
    {"OLL-23", "R2 D (R' U2 R) D' (R' U2 R')"},
    {"OLL-24", "(r U R' U') (r' F R F')"},
    {"OLL-25", "F' (r U R' U') r' F R"},
    {"OLL-26", "R U2 R' U' R U' R'"},
    {"OLL-27", "R U R' U R U2' R'"},
    // Flip 2 edges
    {"OLL-28", "(r U R' U') R (r' U R U' R')"},
    {"OLL-57", "(R U R' U') M' (U R U' r')"},
    // Square
    {"OLL-5", "r' U2 (R U R' U) r"},
    {"OLL-6", "r U2 (R' U' R U') r'"},
    // Square with dot
    {"OLL-35", "R U2' (R2' F R F') (R U2' R')"},
    {"OLL-37", "F (R U' R' U') (R U R' F')"},
    // T shape
    {"OLL-33", "(R U R' U') (R' F R F')"},
    {"OLL-45", "F (R U R' U') F'"},
    // P shape
    {"OLL-31", "(R' U' F) (U R U' R') F' R"},
    {"OLL-32", "R U B' (U' R' U) (l U l')"},
    {"OLL-43", "R' U' F' U F R"},
    {"OLL-44", "f (R U R' U') f'"},
    // W shape
    {"OLL-36", "(L' U' L U') (L' U L U) (r U' r' F)"},
    {"OLL-38", "(R U R' U) (R U' R' U') (R' F R F')"},
    // C shape
    {"OLL-34", "(R U R2' U') (R' F R U) R U' F'"},
    {"OLL-46", "R' U' (R' F R F') U R"},
    // I shape
    {"OLL-51", "f (R U R' U') (R U R' U') f'"},
    {"OLL-52", "(R' F' U' F) U' (R U R' U R)"},
    {"OLL-55", "(R U2' R2') (U' R U' R') U2' (F R F')"},
    {"OLL-56", "(r U r') (U R U' R') (U R U' R') (r U' r')"},
    // Big L shape
    {"OLL-13", "r U' r' (U' r U r') (F' U F)"},
    {"OLL-14", "R' F R (U R' F' R) (F U' F')"},
    {"OLL-15", "(r' U' r) (R' U' R U) (r' U r)"},
    {"OLL-16", "(r U r') (R U R' U') (r U' r')"},
    // Big Z shape
    {"OLL-39", "L F' (L' U' L U) F U' L'"},
    {"OLL-40", "R' F (R U R' U') F' U R"},
    // Small Z shape
    {"OLL-7", "r U (R' U R U2') r'"},
    {"OLL-8", "r' U' (R U' R' U2) r"},
    {"OLL-11", "r' (R2 U R' U R) U2' (R' U M')"},
    {"OLL-12", "M' (R' U' R U' R' U2 R U') r' R"},
    // Small L shape (lightning)
    {"OLL-47", "R' U' l' (U R U' R') (U R U' B) l"},
    {"OLL-48", "F (R U R' U') (R U R' U') F'"},
    {"OLL-49", "r U' r2' U r2 U r2' U' r"},
    {"OLL-50", "r' U r2 U' r2' U' r2 U r'"},
    {"OLL-53", "(r' U' R U') (R' U R U') (R' U2 r)"},
    {"OLL-54", "(r U R' U) (R U' R' U)(R U2' r')"},
    // Small L with dot
    {"OLL-9", "(R U R' U') R' F (R2 U R' U') F'"},
    {"OLL-10", "(R U R' U) (R' F R F') (R U2' R')"},
    // Small Z with dot (awkward)
    {"OLL-29", "(R U R' U') (R U' R') (F' U' F) (R U R')"},
    {"OLL-30", "F U (R U2 R' U') (R U2 R' U') F'"},
    {"OLL-41", "(R U R' U R U2') R' F (R U R' U') F'"},
    {"OLL-42", "(F R' F' R) U2 (R' U' R2 U') (R2' U2' R)"},
    // Dot shape
    {"OLL-1", "(R U2') (R2' F R F') U2' (R' F R F')"},
    {"OLL-2", "F (R U R' U') F' f (R U R' U') f'"},
    {"OLL-3", "f (R U R' U') f' U' F (R U R' U') F'"},
    {"OLL-4", "f (R U R' U') f' U F (R U R' U') F'"},
    {"OLL-17", "(R U R' U) (R' F R F') U2 (R' F R F')"},
    {"OLL-18", "R U2' (R2' F R F') U2 M' (U R U' r')"},
    {"OLL-19", "r' R U (R U R' U') M' (R' F R F')"},
    {"OLL-20", "r' R U (R U R' U') M2' (U R U' r')"}
}};

constexpr std::array<Algo, 41> f2l_algos = {{
    // 4 standard cases
    {"F2L-1", "U (R U' R')"},
    {"F2L-2", "y' U' (R' U R)", "y'"},
    {"F2L-3", "y' (R' U' R)", "y'"},
    {"F2L-4", "(R U R')"},
    // Edge-top = F, corner-top = F
    {"F2L-5", "U' (R U R') U2 (R U' R')"},
    {"F2L-7", "U' (R U2' R') U2 (R U' R')"},
    {"F2L-15", "(R U R') U2 (R U' R' U) (R U' R')"},
    // Edge-top = F, corner-top = R
    {"F2L-10", "U' (R U R' U) (R U R')"},
    {"F2L-12", "(R U' R' U) (R U' R' U2) (R U' R')"},
    {"F2L-14", "U' (R U' R' U) (R U R')"},
    // Edge-top = F, corner-top = D
    {"F2L-19", "U (R U2' R') U (R U' R')"},
    {"F2L-21", "U2 (R U R' U) (R U' R')"},
    {"F2L-17", "(R U2 R') U' (R U R')"},
    {"F2L-23", "U (R U' R' U') (R U' R' U) (R U' R')"},
    // Edge-top = R, corner-top = R
    {"F2L-6", "d (R' U' R) U2' (R' U R)", "y'"},
    {"F2L-8", "d (R' U2 R) U2' (R' U R)", "y'"},
    {"F2L-16", "(R U' R') U2 y' (R' U' R)", "y'"},
    // Edge-top = R, corner-top = F
    {"F2L-9", "d (R' U' R U') (R' U' R)", "y'"},
    {"F2L-11", "U' (R U2' R') d (R' U' R)", "y'"},
    {"F2L-13", "d (R' U R U') (R' U' R)", "y'"},
    // Edge-top = R, corner-top = D
    {"F2L-20", "y' U' (R' U2 R) U' (R' U R)", "y'"},
    {"F2L-22", "y' U2 (R' U' R) U' (R' U R)", "y'"},
    {"F2L-18", "y' (R' U2' R) U (R' U' R)", "y'"},
    {"F2L-24", "U' R U (R2' F R F') (R U' R')"},
    // Corner in slot, Edge-top = F
    {"F2L-25", "U' (R' F R F') (R U R')"},
    {"F2L-27", "(R U' R' U) (R U' R')"},
    {"F2L-30", "(R U R' U') (R U R')"},
    // Corner in slot, Edge-top = R
    {"F2L-26", "U (R U' R') (F R' F' R)"},
    {"F2L-28", "y' (R' U R U') (R' U R)", "y'"},
    {"F2L-29", "y' (R' U' R U) (R' U' R)", "y'"},
    // Edge in slot, correct
    {"F2L-33", "U' (R U' R') U2 (R U' R')"},
    {"F2L-34", "U (R U R') U2 (R U R')"},
    {"F2L-32", "(U R U' R') (U R U' R') (U R U' R')"},
    // Edge in slot, incorrect
    {"F2L-31", "U' (R' F R F') (R U' R')"},
    {"F2L-35", "U2 (R U R') (F R' F' R)"},
    {"F2L-36", "U (F' U' F) U' (R U R')"},
    // Corner & Edges both in slot
    {"F2L-38", "(R U R' U') R U2 R' U' (R U R')"},
    {"F2L-39", "(R U' R' U) (R U2 R' U) (R U' R')"},
    {"F2L-40", "(R U' R') U' (R U' R') d (R' U' R)", "y'"},
    {"F2L-41", "(R U R') U' (R U' R') U2 y' (R' U' R)", "y'"},
    {"F2L-37", "R2' U2' (F R2 F') U2 (R' U R')"},
}};

}

#endif  // CUBIMG_CFOP_H