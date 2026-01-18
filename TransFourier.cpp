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
        vector<vector<comp>> freqA;
        comp temp;
        double tempd;
        double savey;
        double max = 0;
        comp sum;
        double pix1,pix2;
        for (int v = 0; v < h; v++)
        {
            cout << v << endl;
            freq.push_back(vector<comp>(0));
            for (int u = 0; u < w; u++) {
                sum.r = 0;
                sum.i = 0;
                //pix1 = image(u,v,0);
                for (int y = 0; y < h; y++)
                {
                    savey = double(v*y)/h;
                    for (int x = 0; x < w; x++) {
                        pix2 = image(x,y,0) ;
                        tempd = 2*pi*((double(u*x)/w)+(savey));
                        sum.r += pix2*cos(tempd);
                        sum.i -= pix2*sin(tempd);
                    }
                }
                freq[v].push_back(sum);
                tempd = (pow(freq[v][u].r,2) + pow(freq[v][u].i,2));

                if (tempd > max) {
                    max = tempd;
                }
            }
        }
        cout << "XD" << endl;
        freqA = freq;
        for (int i=0;i<h/2;i++)
        {
            for (int j=0;j<w/2;j++) {
                temp = freq[i][j];
                freq[i][j] = freqA[i+h/2][j+w/2];
                freq[i+h/2][j+w/2] = temp;
            }
        }
        for (int i=h/2;i<h;i++)
        {
            for (int j=0;j<w/2;j++) {
                temp = freq[i][j];
                freq[i][j] = freqA[i-h/2][j+w/2];
                freq[i-h/2][j+w/2] = temp;
            }
        }
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
        freqToSpatial(freq);
        return freq;
    }

void freqToSpatial(vector<vector<comp>> image) {
        int w = image[0].size();
        int h = image.size();
        vector<vector<double>> freq;
        vector<vector<double>> freqB;
        vector<vector<comp>> freqA;
        double tempd;
        comp temp;
        double savey;
        double max = 0;
        double sum;
        comp pix;
        freqA = image;
        for (int i=0;i<h/2;i++)
        {
            for (int j=0;j<w/2;j++) {
                temp = image[i][j];
                image[i][j] = freqA[i+h/2][j+w/2];
                image[i+h/2][j+w/2] = temp;
            }
        }
        for (int i=h/2;i<h;i++)
        {
            for (int j=0;j<w/2;j++) {
                temp = image[i][j];
                image[i][j] = freqA[i-h/2][j+w/2];
                image[i-h/2][j+w/2] = temp;
            }
        }
        for (int v = 0; v < h; v++)
        {
            cout << v << endl;
            freq.push_back(vector<double>(0));
            for (int u = 0; u < w; u++) {
                sum = 0;
                //pix1 = image(u,v,0);
                for (int y = 0; y < h; y++)
                {
                    savey = double(v*y)/h;
                    for (int x = 0; x < w; x++) {
                        pix = image[x][y];
                        tempd = 2*pi*((double(u*x)/w)+(savey));
                        sum += pix.r*cos(tempd) - pix.i*sin(tempd);
                    }
                }
                sum = sum / (w * h);
                freq[v].push_back(sum);
            }
        }
        freqB = freq;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                freq[x][h - 1 - y] = freqB[y][x];
            }
        }
        cout << "XD" << endl;
        //frequency domain computed
        cimg_library::CImg<unsigned char> outputImage(w, h, 1, 1, 0);;
        int value;
        for (int v = 0; v < h; v++) {
            for (int u = 0; u < w; u++) {
                value =  clamp((int)round(freq[v][u]),0,255);
                outputImage(u,v,0) = value;
            }
        }
        outputImage.save("spatialSpectrum.bmp");
        cout << "Saved" << endl;
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
    for (int v = 0; v < h; v++)
    {
        //cout << v << endl;
        mem.clear();
        for (int u = 0; u < w; u++) {
            temp.r = image(u, v, 0) //* ((u + v) % 2 ? -1.0 : 1.0)
            ;
            temp.i = 0;
            mem.push_back(temp);
        }
        stfFFT(mem,1);
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
        stfFFT(mem,1);
        freq.push_back(mem);
    }
    for (int v =0; v < h; v++)
    {
        for (int u = 0; u < w; u++) {
                val = freq[v][u].r * freq[v][u].r +
                        freq[v][u].i * freq[v][u].i;
                if (val > max) {
                    max = val;
                }
        }
    }
    cout << "XD" << endl;
    freqA = freq;
    for (int i=0;i<h;i++) {
        for (int j=0;j<w;j++) {
            freq[i][j] = freqA[j][i];
        }
    }
    freqA = freq;
    for (int i=0;i<h/2;i++)
    {
        for (int j=0;j<w/2;j++) {
            temp = freq[i][j];
            freq[i][j] = freqA[i+h/2][j+w/2];
            freq[i+h/2][j+w/2] = temp;
        }
    }
    for (int i=h/2;i<h;i++)
    {
        for (int j=0;j<w/2;j++) {
            temp = freq[i][j];
            freq[i][j] = freqA[i-h/2][j+w/2];
            freq[i-h/2][j+w/2] = temp;
        }
    }

    //frequency domain computed
    cimg_library::CImg<unsigned char> outputImage = image;
    int value;
    max = log(1+sqrt(max));
    for (int v = 0; v < h; v++) {
        for (int u = 0; u < w; u++) {
            value = sqrt(freq[v][u].r*freq[v][u].r + freq[v][u].i*freq[v][u].i);
            value = log(1 + value);
            value = (int) (255*value/max);
            outputImage(u,v,0) = value;
            outputImage(u,v,1) = value;
            outputImage(u,v,2) = value;
        }
    }
    outputImage.save("freqSpectrum.bmp");
    cout << "Saved" << endl;
    freqToSpatialFast(freq);
    return freq;
}
void stfFFT(vector<comp>& a, bool foreward) {
    int n = a.size();
    if (n <= 1) return;

    vector<comp> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }

    stfFFT(a0, foreward);
    stfFFT(a1, foreward);

    double angle = 2.0 * M_PI / n * (foreward ? -1 : 1);
    comp w(1, 0), wn(cos(angle), sin(angle));

    for (int i = 0; i < n / 2; i++) {
        comp u = a0[i];
        comp v = w * a1[i];
        a[i] = u + v;
        a[i + n / 2] = u - v;
        w = w * wn;
    }
}
int bitReverse(int index, int log2n) {
    int reversed = 0;
    for (int i = 0; i < log2n; i++) {
        reversed <<= 1;
        reversed |= (index & 1);
        index >>= 1;
    }
    return reversed;
}
void bitReversal(vector<comp>& data) {
    int n = data.size();
    int log2n = 0;
    while ((1 << log2n) < n) log2n++;

    for (int i = 0; i < n; i++) {
        int j = bitReverse(i, log2n);
        if (i < j) {
            swap(data[i], data[j]);
        }
    }
}

void freqToSpatialFast(vector<vector<comp>> image) {
    int w = image[0].size();
    int h = image.size();
    vector<vector<comp>> freqA;
    vector<vector<comp>> freq;
    comp temp;
    double val;
    double savey;
    double max = 0;
    comp sum;
    double pix1,pix2;
    freqA = image;
    for (int i=0;i<h/2;i++)
    {
        for (int j=0;j<w/2;j++) {
            temp = image[i][j];
            image[i][j] = freqA[i+h/2][j+w/2];
            image[i+h/2][j+w/2] = temp;
        }
    }
    for (int i=h/2;i<h;i++)
    {
        for (int j=0;j<w/2;j++) {
            temp = image[i][j];
            image[i][j] = freqA[i-h/2][j+w/2];
            image[i-h/2][j+w/2] = temp;
        }
    }
    freqA.clear();
    for (int v = 0; v < h; v++)
    {
        //cout << v << endl;
        mem.clear();
        for (int u = 0; u < w; u++) {
            temp.r = image[v][u].r //* ((u + v) % 2 ? -1.0 : 1.0)
            ;
            temp.i = image[v][u].i;
            mem.push_back(temp);
        }
        stfFFT(mem,0);
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
        stfFFT(mem,0);
        freq.push_back(mem);
    }
    cout << "XD" << endl;
    freqA = freq;
    for (int i=0;i<h;i++) {
        for (int j=0;j<w;j++) {
            freq[i][j] = freqA[h-i-1][w-j-1];
        }
    }
    freqA = freq;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            freq[x][h - 1 - y] = freqA[y][x];
        }
    }

    //frequency domain computed
    cout << "XD" << endl;
    //frequency domain computed
    cimg_library::CImg<unsigned char> outputImage(w, h, 1, 1, 0);;
    int value;
    for (int v = 0; v < h; v++) {
        for (int u = 0; u < w; u++) {
            value =  clamp((int)round(freq[v][u].r/(w*h)),0,255);
            outputImage(u,v,0) = value;
        }
    }
    outputImage.save("spatialSpectrum.bmp");
    cout << "Saved" << endl;
}




