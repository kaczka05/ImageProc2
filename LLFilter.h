//
// Created by HPOMEN on 16.11.2025.
//

#ifndef LLFILTER_H
#define LLFILTER_H



#include "CImg.h"


using namespace cimg_library;


// Callable function version of LL filter
void ll_filter(CImg<unsigned char>& img, int mask_size);



#endif //LLFILTER_H
