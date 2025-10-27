//
// Created by damia on 27.10.2025.
//

#include "histogram.h"
#include "CImg.h"
using namespace std;
using namespace cimg_library;

void create_histogram(CImg<unsigned char>& image) {
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    int histHeight = 100;
    CImg<unsigned char> histImg(256, 100, 1, 3, 255); // white background

    // 3️⃣ Draw vertical bars
    for (int i = 0; i < 256; i++) {
        int barHeight = (int)(histogram[i] * histHeight);
        for (int y = histHeight - 1; y >= histHeight - barHeight; y--) {
            histImg(i, y, 0) = 0;   // Red channel
            histImg(i, y, 1) = 0;   // Green
            histImg(i, y, 2) = 0;   // Blue (→ black bar)
        }
    }
    image.save_bmp("histogram.bmp");
}