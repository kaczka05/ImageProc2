//
// Created by damia on 15.11.2025.
//

#ifndef UNTITLED_REYLEIGH_H
#define UNTITLED_REYLEIGH_H

#include "CImg.h"
#include <math.h>
#include <algorithm>
using namespace std;
    void computeReyleigh(cimg_library::CImg<unsigned char>& image, int gmin, int gmax);
    void analyseHistogram(float histogram[255],float pixelAmount);
    void mean(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void variance(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void standardDeviation(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void variationCoefficient1(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void asymmetryCoefficient(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void flatteningCoefficient(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void variationCoefficient2(cimg_library::CImg<unsigned char>& image,float pixelAmount);
    void informationSourceEnthropy(cimg_library::CImg<unsigned char>& image,float pixelAmount);








#endif //UNTITLED_REYLEIGH_H