#ifndef MORPHOLOGICAL_M4_H
#define MORPHOLOGICAL_M4_H

#include "CImg.h"
#include <vector>
#include <utility>

class MorphologicalM4 {
public:
    using Offset = std::pair<int,int>;


    // B1 foreground
    // B2 background
    struct StructuringElement {
        std::vector<Offset> B1;
        std::vector<Offset> B2;
    };


    static cimg_library::CImg<unsigned char>
    hitOrMiss(const cimg_library::CImg<unsigned char>& A,
              const StructuringElement& B);

    static cimg_library::CImg<unsigned char>
    iterate(const cimg_library::CImg<unsigned char>& A,
            const StructuringElement& B);

    static cimg_library::CImg<unsigned char>
    computeH(const cimg_library::CImg<unsigned char>& A,
             const std::vector<StructuringElement>& elements);

private:
    static bool inBounds(int x, int y, int w, int h);
};

#endif
