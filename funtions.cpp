//
// Created by HPOMEN on 20.10.2025.
//
#include "funtions.h"
#include <set>
using namespace std;

void brightness(CImg<unsigned char>& image, int Iargument){

    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            for (int channel = 0; channel < 3; channel++) {
                float var = image(x, y, channel);
                if (var + Iargument > 255) {
                    image(x,y,channel )= 255;
                }
                else if (var + Iargument < 0) {
                    image(x,y, channel) = 0;
                }
                else {
                    image(x,y,channel) = var+ Iargument;
                }
            }
        }
    }
}

void contrast(CImg<unsigned char>& image, int Iargument){
    float mem[3] = {0,0,0};
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            for (int channel = 0; channel < 3; channel++) {
                float var = image(x, y, channel);
                mem[channel] += var;
            }
        }
    }
    for (int channel = 0; channel < 3; channel++) {
        mem[channel] /= image.width() * image.height();
    }
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            for (int channel = 0; channel < 3; channel++) {
                float var = (image(x, y, channel)- mem[channel])*Iargument+mem[channel];
                if (var > 255) {
                    image(x,y,channel )= 255;
                }
                else if (var < 0) {
                    image(x,y, channel) = 0;
                }
                else {
                    image(x,y,channel) = var;
                }
            }
        }
    }
}

void negative(CImg<unsigned char>& image) {
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            for (int channel = 0; channel < 3; channel++) {
                image(x,y,channel) = 255 - image(x,y,channel) ;
            }
        }
    }
}

void horizontal_flip(CImg<unsigned char>& image) {
    float temp = image(0, 0, 0);

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width()/2; x++) {
            for (int channel = 0; channel < 3; channel++) {
                temp = image(image.width() -1 -x, y, channel);
                image(image.width() -1 -x, y, channel) = image(x, y, channel);
                image(x, y,channel) = temp;
            }

        }
    }
}

void vertical_flip(CImg<unsigned char>& image) {
    float temp = image(0, 0, 0);

    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height()/2; y++) {
            for (int channel = 0; channel < 3; channel++) {
                temp = image(x,image.height() - y -1, channel);
                image(x, image.height() - y -1, channel) = image(x, y, channel);
                image(x, y,channel) = temp;
            }
        }
    }
}

void diagonal_flip(CImg<unsigned char>& image) {
    float temp = image(0, 0, 0);
    CImg<unsigned char> outputImage = image;
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            for (int channel = 0; channel < 3; channel++) {
                outputImage(image.width()-x-1, image.height()-y-1, channel) = image(x, y, channel);
            }
        }
    }
    outputImage.save_bmp("out.bmp");
}

void change_size(CImg<unsigned char> &image, int newW, int newH) {
    int srcW = image.width();
    int srcH = image.height();
    int channels = 3;

    CImg<unsigned char> dst(newW, newH, 1, channels, 0);

    for (int i = 0; i < newH; i++) {        // dst row (y)
        for (int j = 0; j < newW; j++) {    // dst col (x)

            int src_x = (j * srcW) / newW;  // column in source
            int src_y = (i * srcH) / newH;  // row in source


            if (src_x >= srcW) src_x = srcW - 1;
            if (src_y >= srcH) src_y = srcH - 1;

            for (int c = 0; c < channels; c++) {
                dst(j, i, 0, c) = image(src_x, src_y, 0, c);
            }
        }
    }

    image = dst;
}

void adaptive_filter(CImg<unsigned char> &image, int minSize, int maxSize) {
    float temp = image(0, 0, 0);
        multiset<float> values;
        int currSize = minSize;
        int expX,expY;
        float minimum,maximum,median,medTemp;
        CImg<unsigned char> outputImage = image;
        auto setIter = values.begin();
        for (int x = minSize; x < image.width()-minSize; x++) {
            for (int y = minSize; y < image.height()-minSize; y++) {
                for (int channel = 0; channel < 3; channel++) {
                    values.clear();
                    //beginning of filter
                    currSize = minSize;
                    for(int wx = x-currSize ; wx < x+currSize; wx++) { //preparing a set for minSize
                        for(int wy = y-currSize ; wy < y+currSize; wy++) {
                            if (wy >= 0 && wy < image.height() && wx >= 0 && wx < image.width()) {//conditional to not extend the image size
                                values.insert(image(wx, wy, channel));
                            }
                        }
                    }
                    while (true) {//loop where we check the window and extend it
                        minimum = *values.begin();
                        maximum = *values.rbegin();
                        setIter = values.begin();
                        if (values.size()%2 == 0) {//two cases for if the amount of pixels in window is even or odd, to acces set we need an iterator
                            advance(setIter, values.size() / 2 - 1);
                            medTemp = *setIter;
                            ++setIter;
                            median = (*setIter + medTemp) / 2;
                        }
                        else {
                            advance(setIter, values.size() / 2);
                            median = *setIter;
                        }
                        if (median-minimum>0 && median-maximum < 0) { //first condition, is median extream
                            if (image(x, y, channel) - minimum > 0 && image(x, y, channel) - maximum < 0 ) { // second condition, is pixel not extream
                                outputImage(x, y, channel) = image(x, y, channel);
                                break;
                            }
                            else { // pixel is extream
                                //cout << "used median " << minimum << " " << median << " " << maximum <<  endl;
                                outputImage(x, y, channel) = median;
                                break;
                            }
                        }
                        else { // we expand the window to get better median
                            currSize ++;
                        }
                        if (currSize > maxSize) {//we reached the maximum window size
                            outputImage(x, y, channel) = image(x, y, channel);
                            break;
                        }
                        for (int expIter = 0 ; expIter < currSize*2; expIter++) {//expanding the window by the borders, I add the corners and then the pixels assigned to it in clockwise direction
                            expX = x-currSize + expIter;
                            expY = y-currSize;
                            if (expY >= 0 && expY < image.height() && expX >= 0 && expX < image.width()) {//conditional to not extend the image size
                                values.insert(image(expX, expY, channel));
                            }
                            expX = x+currSize;
                            expY = y-currSize + expIter;
                            if (expY >= 0 && expY < image.height() && expX >= 0 && expX < image.width()) {//conditional to not extend the image size
                                values.insert(image(expX, expY, channel));
                            }
                            expX = x+currSize - expIter;
                            expY = y+currSize;
                            if (expY >= 0 && expY < image.height() && expX >= 0 && expX < image.width()) {//conditional to not extend the image size
                                values.insert(image(expX, expY, channel));
                            }
                            expX = x-currSize;
                            expY = y-currSize - expIter;
                            if (expY >= 0 && expY < image.height() && expX >= 0 && expX < image.width()) {//conditional to not extend the image size
                                values.insert(image(expX, expY, channel));
                            }
                        }
                    }
                }
            }
        }
        outputImage.save_bmp("out.bmp");
    }


