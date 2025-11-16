//
// Created by HPOMEN on 16.11.2025.
//

#include "LLFilter.h"
#include "perf.h"
#include <algorithm>
#include <vector>


using namespace perf;
using namespace cimg_library;


void ll_filter(CImg<unsigned char>& img, int mask_size) {
    reset();
    if (mask_size <= 1) return;


    int w = img.width(), h = img.height(), s = img.spectrum();
    int r = mask_size / 2;


    CImg<unsigned char> out = img;


    for (int c = 0; c < s; ++c) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                std::vector<unsigned char> neighborhood;
                for (int yy = y - r; yy <= y + r; ++yy) {
                    int sy = std::min(h - 1, std::max(0, yy));
                    for (int xx = x - r; xx <= x + r; ++xx) {
                        int sx = std::min(w - 1, std::max(0, xx));
                        neighborhood.push_back(img(sx, sy, 0, c));
                    }
                }
                std::sort(neighborhood.begin(), neighborhood.end());
                additions += neighborhood.size(); // approximate sorting cost counted as additions
                multiplications += neighborhood.size(); // rough estimate
                unsigned char val = neighborhood[neighborhood.size()/2];
                out(x,y,0,c) = val;
            }
        }
    }


    add_memory(out.size() + mask_size*mask_size*sizeof(unsigned char));
    img = out;
}