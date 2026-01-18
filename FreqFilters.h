#ifndef FREQFILTERS_H
#define FREQFILTERS_H

#include "CImg.h"
#include <vector>
#include <cmath>
#include "TransFourier.h"

// All functions operate on grayscale CImg<unsigned char> & and rely on the
// TransFourier interface:
//   vector<vector<comp>> spatialToFreq(CImg<unsigned char>& image);
//   vector<vector<comp>> spatialToFreqFast(CImg<unsigned char>& image);
//   void freqToSpatial(vector<vector<comp>> spectrum);
//
// The spectrum layout is assumed to be freq[row=v][col=u] (i.e. freq[H][W]),
// but the code will auto-detect whether DC lives at (0,0) or at the center
// and adjust masks accordingly.

void freqLowPass(cimg_library::CImg<unsigned char>& image, double cutoffRadius);
void freqHighPass(cimg_library::CImg<unsigned char>& image, double cutoffRadius);
void freqBandPass(cimg_library::CImg<unsigned char>& image, double lowRadius, double highRadius);
void freqBandStop(cimg_library::CImg<unsigned char>& image, double lowRadius, double highRadius);

// Directional high-pass: cutoffRadius (remove low freqs) + directionDeg
// directionDeg is angle in degrees (0 points to +x, 90 to +y). Angular tolerance is fixed (~30°).
void freqDirectionalHP(cimg_library::CImg<unsigned char>& image, double cutoffRadius, double directionDeg);

// Phase modifying filter (F6)
// k,l integers define linear phase P(n,m) = exp(j*(-2π*k*n/N - 2π*l*m/M + (k+l)*π))
// where n = row index (0..N-1), m = col index (0..M-1)
void freqPhaseModify(cimg_library::CImg<unsigned char>& image, int k, int l);

#endif // FREQFILTERS_H
