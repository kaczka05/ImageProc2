#ifndef FREQFILTERS_H
#define FREQFILTERS_H

#include "CImg.h"
#include <vector>
#include <cmath>
#include "TransFourier.h"



void freqLowPass(cimg_library::CImg<unsigned char>& image, double cutoffRadius);
void freqHighPass(cimg_library::CImg<unsigned char>& image, double cutoffRadius);
void freqBandPass(cimg_library::CImg<unsigned char>& image, double lowRadius, double highRadius);
void freqBandStop(cimg_library::CImg<unsigned char>& image, double lowRadius, double highRadius);

void freqDirectionalHP(cimg_library::CImg<unsigned char>& image, double cutoffRadius,  const char* maskFilename);

void freqPhaseModify(cimg_library::CImg<unsigned char>& image, int k, int l);

#endif // FREQFILTERS_H
