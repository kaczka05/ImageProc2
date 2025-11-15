//
// Created by damia on 15.11.2025.
//

#include "reyleigh.h"

void computeReyleigh(cimg_library::CImg<unsigned char>& image, int gmin, int gmax) {
    float histogram [256]  = {0} ;
    float new_histogram [256] = {} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    float newIntensity = 0;
    for (int i =0 ;i<256;i++) {
        newIntensity = 0;
        for (int j = 0;j<=i;j++) {
            newIntensity += histogram[j];
        }
        newIntensity = newIntensity / (image.height()*image.width());
        newIntensity = 1 - newIntensity;
        newIntensity = log(max((double)newIntensity, 1.0/(image.height()*image.width()+1.0)));
        newIntensity = ((float)(gmax-gmin)/3.0) * sqrt(-2*newIntensity)+gmin;
        newIntensity = min(newIntensity,(float)255);
        new_histogram[i] = newIntensity;
    }
}