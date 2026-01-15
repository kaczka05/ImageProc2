//
// Created by damia on 15.01.2026.
//

#ifndef UNTITLED_TRANSFOURIER_H
#define UNTITLED_TRANSFOURIER_H

#include "CImg.h"

struct comp {
    double r = 0, i = 0;
};


void spatialToFreq(cimg_library::CImg<unsigned char>& image);
void spatialToFreqFast(cimg_library::CImg<unsigned char>& image);
void freqToSpatial(cimg_library::CImg<unsigned char>& image);
void spatialToFreq(cimg_library::CImg<unsigned char>& image);




#endif //UNTITLED_TRANSFOURIER_H

