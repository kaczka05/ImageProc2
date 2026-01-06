//
// Created by damia on 01.12.2025.
//

#ifndef UNTITLED_REGIONGROWTH_H
#define UNTITLED_REGIONGROWTH_H

#endif //UNTITLED_REGIONGROWTH_H

#include "CImg.h"
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;
struct coor{
    int x,y,region,intensity;
};

struct region {
    int newOwner = -1;
    float regionMean;
    float regionVariance = 0;
    int regionSize = 1;
    vector<coor> pixelsInside;
};
void regionGrowth(cimg_library::CImg<unsigned char>& image, int seedAmount);
float calculateNewParameter(float regionVariance,float regionMean, int regionSize, int pixelIntensity, int parameterId);
bool doesNewParameterFit(region reg, float newRegionParameter);
bool doesNewVarianceFit(region reg, float newVariance);
float calculateNewRegionParameters(float firstRegionMean, float firstRegionVariance, int firstRegionSize, float secondRegionMean, float secondRegionVariance, int secondRegionSize,int parameterId);
float calculateVariance(region reg,region reg2);