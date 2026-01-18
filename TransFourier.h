//
// Created by damia on 15.01.2026.
//

#ifndef UNTITLED_TRANSFOURIER_H
#define UNTITLED_TRANSFOURIER_H

#include "CImg.h"
#include <vector>
using namespace std;
struct comp {
    double r = 0, i = 0;
};


vector<vector<comp>> spatialToFreq(cimg_library::CImg<unsigned char>& image);
vector<vector<comp>> spatialToFreqFast(cimg_library::CImg<unsigned char>& image);
void freqToSpatial(vector<vector<comp>>);
void spatialToFreq(vector<vector<comp>>);
void stfFFT(int start,int size,int depth);


#endif //UNTITLED_TRANSFOURIER_H

