// lowpass_filter.cpp
#include "lowpass_filter.h"  // or "filter.h" if you use that name
#include "perf.h"

#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cmath>

using namespace cimg_library;

// Universal convolution (box mask used here as example). Mirror boundary handling.
void lowpass_filter_universal(CImg<unsigned char>& img, int mask_size) {
    perf::reset();

    if (mask_size <= 1) return;

    int w = img.width();
    int h = img.height();
    int s = img.spectrum();

    int k = mask_size;
    int k2 = k / 2;

    // Build normalized box mask (1/(k*k))
    std::vector<float> mask(k * k, 1.0f / (k * k));
    // count normalization as k*k multiplications
    perf::multiplications += (uint64_t)k * (uint64_t)k;

    // Output image
    CImg<unsigned char> out(w, h, img.depth(), s);

    for (int c = 0; c < s; ++c) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                float acc = 0.0f;
                int mi = 0;
                for (int fy = -k2; fy <= k2; ++fy) {
                    int yy = y + fy;
                    // mirror boundary
                    if (yy < 0) yy = -yy;
                    if (yy >= h) yy = 2 * h - yy - 2;

                    for (int fx = -k2; fx <= k2; ++fx) {
                        int xx = x + fx;
                        if (xx < 0) xx = -xx;
                        if (xx >= w) xx = 2 * w - xx - 2;

                        float pix = img(xx, yy, 0, c);
                        float m = mask[mi++];
                        acc += pix * m;
                        // count one multiplication and one addition per mask element
                        perf::multiplications += 1;
                        perf::additions += 1;
                    }
                }
                int value = (int)(acc + 0.5f);
                if (value < 0) value = 0;
                if (value > 255) value = 255;
                out(x, y, 0, c) = (unsigned char)value;
            }
        }
    }

    // estimate extra memory: output image + mask array
    perf::add_memory((size_t)out.size() + mask.size() * sizeof(float));

    // move output back to input
    img = out;
    //out.save_bmp("out.bmp");
}

// Optimized box filter using integral image (one integral per channel computed in reusable buffer)
void lowpass_filter_optimized(CImg<unsigned char>& img, int mask_size) {
    perf::reset();

    if (mask_size <= 1) return;

    int w = img.width();
    int h = img.height();
    int s = img.spectrum();

    int k = mask_size;
    int k2 = k / 2;

    // allocate integral image buffer of size (w+1)*(h+1) of 64-bit ints
    size_t integral_w = (size_t)w + 1;
    size_t integral_h = (size_t)h + 1;
    size_t integral_pixels = integral_w * integral_h;
    std::vector<uint64_t> integral(integral_pixels);

    // account for integral buffer memory
    perf::add_memory(integral_pixels * sizeof(uint64_t));

    CImg<unsigned char> out(w, h, img.depth(), s);

    for (int c = 0; c < s; ++c) {
        // zero integral
        std::fill(integral.begin(), integral.end(), 0ULL);

        // build integral image: integral[(y+1)*(w+1) + (x+1)] = sum of pixels in rect (0,0)-(x,y)
        for (int y = 0; y < h; ++y) {
            uint64_t row_sum = 0ULL;
            for (int x = 0; x < w; ++x) {
                row_sum += (uint64_t)img(x, y, 0, c);
                // above cell:
                uint64_t above = integral[(size_t)y * integral_w + (size_t)(x + 1)];
                integral[(size_t)(y + 1) * integral_w + (size_t)(x + 1)] = above + row_sum;
                // for building: count one addition (row_sum += pix) already done above
                perf::additions += 1;
            }
        }

        // compute output using integral image
        for (int y = 0; y < h; ++y) {
            int y0 = std::max(0, y - k2);
            int y1 = std::min(h - 1, y + k2);
            for (int x = 0; x < w; ++x) {
                int x0 = std::max(0, x - k2);
                int x1 = std::min(w - 1, x + k2);

                // indices in integral are offset by +1
                size_t A = (size_t)y0 * integral_w + (size_t)x0;         // top-left
                size_t B = (size_t)y0 * integral_w + (size_t)(x1 + 1);   // top-right
                size_t C = (size_t)(y1 + 1) * integral_w + (size_t)x0;   // bottom-left
                size_t D = (size_t)(y1 + 1) * integral_w + (size_t)(x1 + 1); // bottom-right

                uint64_t sum = integral[D] + integral[A] - integral[B] - integral[C];
                // count three additions/subtractions
                perf::additions += 3;

                int area = (x1 - x0 + 1) * (y1 - y0 + 1);
                // count division as one multiplication (approximate)
                perf::multiplications += 1;

                int value = (int)((double)sum / (double)area + 0.5);
                if (value < 0) value = 0;
                if (value > 255) value = 255;
                out(x, y, 0, c) = (unsigned char)value;
            }
        }
    }

    img = out;
    //out.save_bmp("out.bmp");
}
