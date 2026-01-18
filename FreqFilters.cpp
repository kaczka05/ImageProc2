#include "FreqFilters.h"
#include <algorithm>
#include <limits>

using namespace cimg_library;
using std::vector;

// Helper: magnitude squared
static inline double mag2(const comp &c) {
    return c.r * c.r + c.i * c.i;
}

// Helper: pick center (cx,cy) used for radial/angle computations.
// If spectrum magnitude at center is larger than at (0,0), assume DC is centered;
// otherwise assume DC at (0,0). This makes masks robust to either layout.
static void decideCenter(const vector<vector<comp>>& F, int W, int H, int &cx, int &cy) {
    cx = W / 2;
    cy = H / 2;
    // defensive check: if sizes mismatch, fallback to (0,0)
    if ((int)F.size() != H || (int)F[0].size() != W) {
        cx = 0; cy = 0;
        return;
    }
    double magCorner = mag2(F[0][0]);              // DC at (0,0) candidate
    double magCenter = mag2(F[cy][cx]);            // DC at center candidate
    // choose the location that has the larger DC magnitude (robust heuristic)
    if (magCenter > magCorner) {
        // keep cx,cy as center
    } else {
        cx = 0; cy = 0;
    }
}

// Apply a binary mask to the spectrum in place.
// maskFunc should set F[v][u] = {0,0} for frequencies to remove.
static void applyMaskAndReturn(vector<vector<comp>>& F) {
    // After modification, hand back to the provided freqToSpatial
    // (freqToSpatial is declared in TransFourier.h and must be implemented
    //  to reconstruct the image from the modified spectrum).
    freqToSpatial(F);
}

// F1: Low-pass: keep frequencies with distance <= cutoffRadius
void freqLowPass(CImg<unsigned char>& image, double cutoffRadius) {
    int W = image.width();
    int H = image.height();

    // get spectrum (non-fast, consistent row-major freq[v][u])
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

// F2: High-pass: remove frequencies with distance <= cutoffRadius (keep high freq)
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

// F3: Band-pass: keep only frequencies with lowRadius <= d <= highRadius
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

// F4: Band-stop: remove frequencies with lowRadius <= d <= highRadius
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

// F5: Directional high-pass edge detector.
// remove low freqs (< cutoffRadius) and then keep frequencies whose angle is
// within +/- ANG_TOL degrees of directionDeg.
void freqDirectionalHP(CImg<unsigned char>& image, double cutoffRadius, double directionDeg) {
    const double ANG_TOL_DEG = 30.0; // angular tolerance (±30°). Fixed; change if needed.

    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);

    int cx, cy;
    decideCenter(F, W, H, cx, cy);

    double r2 = cutoffRadius * cutoffRadius;
    double theta = directionDeg * M_PI / 180.0;
    double tol = ANG_TOL_DEG * M_PI / 180.0;

    for (int v = 0; v < H; ++v) {
        double dv = double(v - cy);
        for (int u = 0; u < W; ++u) {
            double du = double(u - cx);
            double d2 = du*du + dv*dv;

            // remove low-frequency region
            if (d2 <= r2) {
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
                continue;
            }

            // compute angle of this frequency vector
            double angle = std::atan2(dv, du);          // range (-pi, pi]
            // compute smallest angular distance to requested direction (wrap-aware)
            double diff = std::fmod(angle - theta + 3*M_PI, 2*M_PI) - M_PI;
            if (std::fabs(diff) > tol) {
                // not within the target directional wedge -> discard
                F[v][u].r = 0.0;
                F[v][u].i = 0.0;
            }
        }
    }

    applyMaskAndReturn(F);
}

// F6: Phase modifying filter: multiply each frequency element by a unit-magnitude complex
// number P(n,m) = exp(j*( -2π*k*n/N - 2π*l*m/M + (k+l)π ))
void freqPhaseModify(CImg<unsigned char>& image, int k, int l) {
    int W = image.width();
    int H = image.height();
    vector<vector<comp>> F = spatialToFreq(image);

    // n = row index (0..H-1), m = col index (0..W-1)
    for (int n = 0; n < H; ++n) {
        for (int m = 0; m < W; ++m) {
            double phase = -2.0 * M_PI * k * n / double(H)
                           -2.0 * M_PI * l * m / double(W)
                           + (k + l) * M_PI;
            double c = std::cos(phase);
            double s = std::sin(phase);

            double r = F[n][m].r;
            double i = F[n][m].i;

            // complex multiply (r + j i) * (c + j s)
            F[n][m].r = r * c - i * s;
            F[n][m].i = r * s + i * c;
        }
    }

    applyMaskAndReturn(F);
}
