#include "FreqFilters.h"
#include <algorithm>
#include <limits>

using namespace cimg_library;
using std::vector;


static inline double mag2(const comp &c) {
    return c.r * c.r + c.i * c.i;
}


static void decideCenter(const vector<vector<comp>>& F, int W, int H, int &cx, int &cy) {
    cx = W / 2;
    cy = H / 2;
    if ((int)F.size() != H || (int)F[0].size() != W) {
        cx = 0; cy = 0;
        return;
    }
    double magCorner = mag2(F[0][0]);              // DC at (0,0) candidate
    double magCenter = mag2(F[cy][cx]);            // DC at center candidate
    // choose the location with biger dc
    if (magCenter > magCorner) {

    } else {
        cx = 0; cy = 0;
    }
}


static void applyMaskAndReturn(vector<vector<comp>>& F) {

    freqToSpatial(F);
}


void freqLowPass(CImg<unsigned char>& image, double cutoffRadius) {
    int W = image.width();
    int H = image.height();


    vector<vector<comp>> F = spatialToFreq(image);

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double r2 = cutoffRadius * cutoffRadius;
    for (int v = 0; v < H; ++v) {
        int dv = v - cy;
        for (int u = 0; u < W; ++u) {
            int du = u - cx;
            double d2 = double(du*du + dv*dv);
            if (d2 > r2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
            }
        }
    }

    applyMaskAndReturn(F);
}


void freqHighPass(CImg<unsigned char>& image, double cutoffRadius) {
    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double r2 = cutoffRadius * cutoffRadius;
    for (int v = 0; v < H; ++v) {
        int dv = v - cy;
        for (int u = 0; u < W; ++u) {
            int du = u - cx;
            double d2 = double(du*du + dv*dv);
            if (d2 <= r2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
            }
        }
    }

    applyMaskAndReturn(F);
}


void freqBandPass(CImg<unsigned char>& image, double lowRadius, double highRadius) {
    if (lowRadius < 0) lowRadius = 0;
    if (highRadius < lowRadius) std::swap(lowRadius, highRadius);

    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double low2 = lowRadius * lowRadius;
    double high2 = highRadius * highRadius;
    for (int v = 0; v < H; ++v) {
        int dv = v - cy;
        for (int u = 0; u < W; ++u) {
            int du = u - cx;
            double d2 = double(du*du + dv*dv);
            if (d2 < low2 || d2 > high2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
            }
        }
    }

    applyMaskAndReturn(F);
}


void freqBandStop(CImg<unsigned char>& image, double lowRadius, double highRadius) {
    if (lowRadius < 0) lowRadius = 0;
    if (highRadius < lowRadius) std::swap(lowRadius, highRadius);

    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double low2 = lowRadius * lowRadius;
    double high2 = highRadius * highRadius;
    for (int v = 0; v < H; ++v) {
        int dv = v - cy;
        for (int u = 0; u < W; ++u) {
            int du = u - cx;
            double d2 = double(du*du + dv*dv);
            if (d2 >= low2 && d2 <= high2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
            }
        }
    }

    applyMaskAndReturn(F);
}


void freqDirectionalHP(CImg<unsigned char>& image,
                       double cutoffRadius,
                       const char* maskFilename)
{
    int W = image.width();
    int H = image.height();

    vector<vector<comp>> F = spatialToFreq(image);


    CImg<unsigned char> mask(maskFilename);


    if (mask.width() != W || mask.height() != H) {
        mask.resize(W, H, 1, 1);
    }

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double r2 = cutoffRadius * cutoffRadius;

    for (int v = 0; v < H; ++v) {
        int dv = v - cy;
        for (int u = 0; u < W; ++u) {
            int du = u - cx;
            double d2 = double(du*du + dv*dv);


            if (d2 <= r2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
                continue;
            }

            //mask value in [0,1]
            double m = mask(u, v, 0, 0) / 255.0;

            //attenuation
            F[v][u].r *= m;
            F[v][u].i *= m;


            int us = (2 * cx - u + W) % W;
            int vs = (2 * cy - v + H) % H;

            F[vs][us].r *= m;
            F[vs][us].i *= m;
        }
    }

    applyMaskAndReturn(F);
}





// number P(n,m) = exp(j*( -2π*k*n/N - 2π*l*m/M + (k+l)π ))
void freqPhaseModify(CImg<unsigned char>& image, int k, int l) {
    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);


    for (int n = 0; n < H; ++n) {
        for (int m = 0; m < W; ++m) {
            double phase = -2.0 * M_PI * k * n / double(H)
                           -2.0 * M_PI * l * m / double(W)
                           + (k + l) * M_PI;
            double c = std::cos(phase);
            double s = std::sin(phase);

            double r = F[n][m].r;
            double i = F[n][m].i;

            F[n][m].r = r * c - i * s;
            F[n][m].i = r * s + i * c;
        }
    }

    applyMaskAndReturn(F);
}
