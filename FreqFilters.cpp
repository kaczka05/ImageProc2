#include "FreqFilters.h"
#include <functional>
#include "TransFourier.h"


using namespace cimg_library;
using std::vector;

/*static inline double dist(int u, int v, int cx, int cy) {
    double du = u - cx;
    double dv = v - cy;
    return std::sqrt(du * du + dv * dv);
}

// ===== Generic spectrum loop helper =====
static void applyAndInvert(
    cimg_library::CImg<unsigned char>& image,
    const std::function<void(std::vector<std::vector<comp>>&, int, int)>& maskFunc)
{
    int W = image.width();
    int H = image.height();

    std::vector<std::vector<comp>> F(H, std::vector<comp>(W));

    fft2D(image, F);
    maskFunc(F, W, H);
    ifft2D(F, image);
}


// ===== (F1) Low-pass =====
void freqLowPass(CImg<unsigned char>& image, double cutoff) {
    applyAndInvert(image,
        [cutoff](vector<vector<comp>>& F, int W, int H) {
            int cx = W / 2, cy = H / 2;
            for (int v = 0; v < H; ++v)
                for (int u = 0; u < W; ++u)
                    if (dist(u, v, cx, cy) > cutoff)
                        F[v][u] = {0, 0};
        });
}

// ===== (F2) High-pass =====
void freqHighPass(CImg<unsigned char>& image, double cutoff) {
    applyAndInvert(image,
        [cutoff](vector<vector<comp>>& F, int W, int H) {
            int cx = W / 2, cy = H / 2;
            for (int v = 0; v < H; ++v)
                for (int u = 0; u < W; ++u)
                    if (dist(u, v, cx, cy) < cutoff)
                        F[v][u] = {0, 0};
        });
}

// ===== (F3) Band-pass =====
void freqBandPass(CImg<unsigned char>& image,
                  double lowCut, double highCut)
{
    applyAndInvert(image,
        [lowCut, highCut](vector<vector<comp>>& F, int W, int H) {
            int cx = W / 2, cy = H / 2;
            for (int v = 0; v < H; ++v)
                for (int u = 0; u < W; ++u) {
                    double d = dist(u, v, cx, cy);
                    if (d < lowCut || d > highCut)
                        F[v][u] = {0, 0};
                }
        });
}

// ===== (F4) Band-stop =====
void freqBandStop(CImg<unsigned char>& image,
                  double lowCut, double highCut)
{
    applyAndInvert(image,
        [lowCut, highCut](vector<vector<comp>>& F, int W, int H) {
            int cx = W / 2, cy = H / 2;
            for (int v = 0; v < H; ++v)
                for (int u = 0; u < W; ++u) {
                    double d = dist(u, v, cx, cy);
                    if (d >= lowCut && d <= highCut)
                        F[v][u] = {0, 0};
                }
        });
}

// ===== (F5) Directional high-pass =====
void freqDirectionalHP(CImg<unsigned char>& image,
                       double cutoff,
                       double directionDeg)
{
    double theta = directionDeg * M_PI / 180.0;

    applyAndInvert(image,
        [cutoff, theta](vector<vector<comp>>& F, int W, int H) {
            int cx = W / 2, cy = H / 2;

            for (int v = 0; v < H; ++v)
                for (int u = 0; u < W; ++u) {
                    double du = u - cx;
                    double dv = v - cy;
                    double d = std::sqrt(du * du + dv * dv);
                    if (d < cutoff) {
                        F[v][u] = {0, 0};
                        continue;
                    }
                    double angle = std::atan2(dv, du);
                    if (std::fabs(std::cos(angle - theta)) < 0.7)
                        F[v][u] = {0, 0};
                }
        });
}

// ===== (F6) Phase modifying filter =====
void freqPhaseModify(CImg<unsigned char>& image, int k, int l) {
    int W = image.width();
    int H = image.height();

    vector<vector<comp>> F(H, vector<comp>(W));
    fft2D(image, F);

    for (int n = 0; n < H; ++n)
        for (int m = 0; m < W; ++m) {
            double phase =
                -2.0 * M_PI * k * n / H
                -2.0 * M_PI * l * m / W
                + (k + l) * M_PI;

            double c = std::cos(phase);
            double s = std::sin(phase);

            double r = F[n][m].r;
            double i = F[n][m].i;

            F[n][m].r = r * c - i * s;
            F[n][m].i = r * s + i * c;
        }

    ifft2D(F, image);
}
*/