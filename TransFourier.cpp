//
// Created by damia on 15.01.2026.
//

    #include "TransFourier.h"
    #include <numbers>
    #include <math.h>
    #include <iostream>

    using namespace std;
    using namespace numbers;
    vector<vector<comp>> spatialToFreq(cimg_library::CImg<unsigned char>& image) {
        int w = image.width();
        int h = image.height();
        vector<vector<comp>> freq;
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
                        pix2 = image(x,y,0)  *((x+y)%2 ? -1.0 : 1.0);
                        temp = 2*pi*((double(u*x)/w)+(savey));
                        sum.r += pix2*cos(temp);
                        sum.i -= pix2*sin(temp);
                    }
                }
                freq[v].push_back(sum);
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
        return freq;
    }

vector<comp> mem;
vector<vector<comp>> spatialToFreqFast(cimg_library::CImg<unsigned char>& image) {
    int w = image.width();
    int h = image.height();
    vector<vector<comp>> freqA;
    vector<vector<comp>> freq;
    comp temp;
    double val;
    double savey;
    double max = 0;
    comp sum;
    double pix1,pix2;
    cout << h << w << endl;
    for (int v = 0; v < h; v++)
    {
        //cout << v << endl;
        mem.clear();
        for (int u = 0; u < w; u++) {
            temp.r = image(u, v, 0) * ((u + v) % 2 ? -1.0 : 1.0);
            temp.i = 0;
            mem.push_back(temp);
        }
        stfFFT(0,w,1);
        freqA.push_back(mem);
    }
    for (int u = 0; u < w; u++) {
        //cout << u << endl;
        mem.clear();
        for (int v = 0; v < h; v++) {
            temp.r = freqA[v][u].r;
            temp.i = freqA[v][u].i;
            mem.push_back(temp);
        }
        stfFFT(0,h,1);
        freq.push_back(mem);
        for (int v = 0; v < h; v++) {
            val = freq[u][v].r * freq[u][v].r +
                 freq[u][v].i * freq[u][v].i;
            if (val > max) {
                max = val;
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
    return freq;
}
void stfFFT(int start,int size,int depth) {
    if (size > 2) {
        stfFFT(start,size/2,depth*2);
        stfFFT(start+depth,size/2,depth*2);
    }
    comp temp, temp2;
    double angle;
    for (int i = start; i < size + start; i+=2*depth) {
        angle = 2 * pi * ((i-start)/depth) / size;
        temp.r  = mem[i].r + mem[i+depth].r*cos(angle) - mem[i+depth].i*sin(angle);
        temp.i  = mem[i].i + mem[i+depth].r*sin(angle) + mem[i+depth].i*cos(angle);
        temp2.r = mem[i].r - mem[i+depth].r*cos(angle) + mem[i+depth].i*sin(angle);
        temp2.i = mem[i].i - mem[i+depth].r*sin(angle) - mem[i+depth].i*cos(angle);

        mem[i] = temp;
        mem[i+depth] = temp2;
    }
}
int bitReverse(int x, int log2n) {
    int n = 0;
    for (int i = 0; i < log2n; i++) {
        n = (n << 1) | (x & 1);
        x >>= 1;
    }
    return n;
}




