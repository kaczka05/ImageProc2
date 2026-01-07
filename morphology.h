#pragma once

#include "CImg.h"
#include <vector>
#include <string>

namespace morphology {

    using namespace cimg_library;


    using StructuringElement = std::vector<std::pair<int,int>>;


    StructuringElement makeStructuringElement(
        const std::string& type,
        int size
    );


    CImg<unsigned char> dilate(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    );

    CImg<unsigned char> erode(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    );

    CImg<unsigned char> opening(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    );

    CImg<unsigned char> closing(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    );


    CImg<unsigned char> hitAndMissEndpoint(
        const CImg<unsigned char>& src
    );

}
