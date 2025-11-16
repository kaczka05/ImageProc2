//
// Created by HPOMEN on 16.11.2025.
//

#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

#include "CImg.h"


using namespace cimg_library;

class lowpass_filter {

};


void lowpass_filter_universal(CImg<unsigned char>& img, int mask_size);
void lowpass_filter_optimized(CImg<unsigned char>& img, int mask_size);
#endif //LOWPASS_FILTER_H
