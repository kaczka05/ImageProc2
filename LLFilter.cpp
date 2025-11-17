
#include "LLFilter.h"
#include "perf.h"
#include <cmath>
#include <algorithm>

using namespace perf;
using namespace cimg_library;

void ll_filter(CImg<unsigned char>& img, double center_exp) {

    reset();

    if (center_exp <= 0.0) center_exp = 1.0;

    int w = img.width();
    int h = img.height();
    int s = img.spectrum();


    const double eps = 1e-9; // use not to div by 0


    CImg<unsigned char> out(w, h, 1, s, 0);

    for (int c = 0; c < s; ++c) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {

                int y_top    = std::max(0, y - 1);
                int y_bottom = std::min(h - 1, y + 1);
                int x_left   = std::max(0, x - 1);
                int x_right  = std::min(w - 1, x + 1);


                double X  = static_cast<double>(img(x, y, 0, c));
                double A1 = static_cast<double>(img(x, y_top, 0, c));    // top
                double A3 = static_cast<double>(img(x, y_bottom, 0, c)); // bottom
                double A5 = static_cast<double>(img(x_left, y, 0, c));   // left
                double A7 = static_cast<double>(img(x_right, y, 0, c));  // right


                double denom = A1 * A3 * A5 * A7;
                if (denom <= 0.0) denom = eps;


                double num = std::pow(X, center_exp);
                if (num <= 0.0) num = eps;


                double ratio = num / denom;
                if (ratio <= 0.0) ratio = eps;
                double val = 0.25 * std::fabs(std::log(ratio));


                int outv = static_cast<int>(val + 0.5);
                if (outv < 0) outv = 0;
                if (outv > 255) outv = 255;
                out(x, y, 0, c) = static_cast<unsigned char>(outv) ;



            }
        }
    }





    img = out;
}
