#ifndef FREQFILTERS_H
#define FREQFILTERS_H

#include "CImg.h"
#include <vector>
#include "TransFourier.h"
#include <cmath>

// Reuse existing complex structure

/*
 Assumed external FFT interface (to be implemented elsewhere)

 - spectrum is size [H][W]
 - image is grayscale (1 channel)

 These functions MUST exist at link time.
*/
void fft2D(const cimg_library::CImg<unsigned char>& spatial,
           std::vector<std::vector<comp>>& spectrum);

void ifft2D(const std::vector<std::vector<comp>>& spectrum,
            cimg_library::CImg<unsigned char>& spatial);

// ===== Filters =====

// (F1) Low-pass (high-cut)
void freqLowPass(cimg_library::CImg<unsigned char>& image, double cutoff);

// (F2) High-pass (low-cut)
void freqHighPass(cimg_library::CImg<unsigned char>& image, double cutoff);

// (F3) Band-pass
void freqBandPass(cimg_library::CImg<unsigned char>& image,
                  double lowCut, double highCut);

// (F4) Band-stop (band-cut)
void freqBandStop(cimg_library::CImg<unsigned char>& image,
                  double lowCut, double highCut);

// (F5) Directional high-pass edge detector
// direction in degrees: 0, 45, 90, 135
void freqDirectionalHP(cimg_library::CImg<unsigned char>& image,
                       double cutoff,
                       double directionDeg);

// (F6) Phase modifying filter
// P(n,m) = exp(j * ( -2πkn/N - 2πlm/M + (k+l)π ))
void freqPhaseModify(cimg_library::CImg<unsigned char>& image,
                     int k, int l);

#endif
