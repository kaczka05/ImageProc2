//
// Created by damia on 15.01.2026.
//

    #include "TransFourier.h"
    #include <numbers>
    #include <math.h>
    #include <iostream>
    using namespace std;
    using namespace numbers;
    void spatialToFreq(cimg_library::CImg<unsigned char>& image) {
        int w = image.width();
        int h = image.height();
        static comp freq[512][512];
        double temp;
        double savey;
        double max = 0;
        comp sum;
        double pix1,pix2;
        for (int v = 0; v < h; v++)
        {
            cout << v << endl;
            for (int u = 0; u < w; u++) {
                sum.r = 0;
                sum.i = 0;
                //pix1 = image(u,v,0);
                for (int y = 0; y < h; y++)
                {
                    savey = double(v*y)/h;
                    for (int x = 0; x < w; x++) {
                        pix2 = image(x,y,0) /* *((x+y)%2 ? -1.0 : 1.0)*/;
                        temp = 2*pi*((double(u*x)/w)+(savey));
                        sum.r += pix2*cos(temp);
                        sum.i -= pix2*sin(temp);
                    }
                }
                freq[v][u] = sum;
                temp = (pow(freq[v][u].r,2) + pow(freq[v][u].i,2));
                if (temp > max) {
                    max = temp;
                }
            }
        }
        cout << "XD" << endl;
        //frequency domain computed
        cimg_library::CImg<unsigned char> outputImage = image;
        int value;
        max = log(1+sqrt(max));
        for (int v = 0; v < h; v++) {
            for (int u = 0; u < w; u++) {
                value = log(1+sqrt(pow(freq[v][u].r,2) + pow(freq[v][u].i,2)));
                value = (int) (255*value/max);
                outputImage(u,v,0) = value;
                outputImage(u,v,1) = value;
                outputImage(u,v,2) = value;
            }
        }
        outputImage.save("freqSpectrum.bmp");
        cout << "Saved" << endl;
    }

