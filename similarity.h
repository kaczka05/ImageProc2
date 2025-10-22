//
// Created by HPOMEN on 20.10.2025.
//

#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "CImg.h"
#include <limits>

struct SimilarityMetrics {
    double MSE;
    double PMSE;
    double SNR;   // in dB
    double PSNR;  // in dB
    double MD;    // maximum absolute difference
};

// Compute all metrics. Images must have same size & channels.
SimilarityMetrics compute_similarity(const cimg_library::CImg<unsigned char>& original,
                                     const cimg_library::CImg<unsigned char>& processed);


#endif //SIMILARITY_H
