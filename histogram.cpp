//
// Created by damia on 27.10.2025.
//

#include "histogram.h"
#include "CImg.h"
using namespace std;
using namespace cimg_library;

void create_histogram(CImg<unsigned char>& image) {
    int max = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int x = 0 ; x < 256 ; x++) {
        if ( histogram[x] > max ) {
            max = histogram[x];
        }
    }
    int histHeight = 300;
    CImg<unsigned char> histImg(256, 300, 1, 3, 255); // white background


    for (int i = 0; i < 256; i++) {
        int barHeight = (histogram[i]*histHeight)/(max+1);
        for (int j=0; j<=barHeight; j++) {

            histImg(i, histHeight-barHeight-1+j, 0) = 0;   // Red channel
            histImg(i, histHeight-barHeight-1+j, 1) = 0;   // Green
            histImg(i, histHeight-barHeight-1+j, 2) = 0;   // Blue (→ black bar)
        }

    }
    histImg.save_bmp("histogram.bmp");
}