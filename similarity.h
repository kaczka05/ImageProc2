//
// Created by HPOMEN on 20.10.2025.
//

#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "CImg.h"
#include <limits>



// Compute all metrics. Images must have same size & channels.
void compare_similarity(const cimg_library::CImg<unsigned char>& original,
                                     const cimg_library::CImg<unsigned char>& processed);


#endif //SIMILARITY_H
