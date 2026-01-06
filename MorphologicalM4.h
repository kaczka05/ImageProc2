#pragma once
#include "CImg.h"
#include <vector>
#include <utility>

class MorphologicalM4 {
public:
    struct StructuringElement {
        std::vector<std::pair<int,int>> B1; // foreground offsets
        std::vector<std::pair<int,int>> B2; // background offsets
    };

    // Main entry: computes H(A) = D1 ∪ D2 ∪ D3 ∪ D4
    static cimg_library::CImg<unsigned char>
    computeH(const cimg_library::CImg<unsigned char>& A,
             const std::vector<StructuringElement>& elements);

private:
    static cimg_library::CImg<unsigned char>
    hitOrMiss(const cimg_library::CImg<unsigned char>& A,
              const StructuringElement& B);

    static cimg_library::CImg<unsigned char>
    iterate(const cimg_library::CImg<unsigned char>& A,
            const StructuringElement& B);

    static bool inBounds(int x, int y, int w, int h);
};
