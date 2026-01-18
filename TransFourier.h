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
    comp operator+(const comp &n)
    {
        comp temp;
        temp.r = r + n.r;
        temp.i = i + n.i;
        return temp;
    }
    comp operator-(const comp &n)
    {
        comp temp;
        temp.r = r - n.r;
        temp.i = i - n.i;
        return temp;
    }
    comp operator*(const comp &n) {
        comp temp;
        temp.r = r * n.r - i * n.i;
        temp.i = r * n.i + i * n.r;
        return temp;
    }

};


vector<vector<comp>> spatialToFreq(cimg_library::CImg<unsigned char>& image);
vector<vector<comp>> spatialToFreqFast(cimg_library::CImg<unsigned char>& image);
void freqToSpatial(vector<vector<comp>>);
void freqToSpatialFast(vector<vector<comp>>);
void stfFFT(vector<comp>& a, bool foreward);
int bitReverse(int index, int log2n);
void bitReversal(vector<comp>& data);


#endif //UNTITLED_TRANSFOURIER_H

