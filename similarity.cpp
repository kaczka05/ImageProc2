#include "CImg.h"
#include "ostream"
#include "iostream"


using namespace std;
using namespace cimg_library;

void compare_similarity(const CImg<unsigned char>& original, const CImg<unsigned char>& processed) {




    int W = original.width();
    int H = original.height();
    int C = original.spectrum();
    long long total = (W) * H * C;

    double sum_diff_sq = 0.0;
    double sum_signal_sq = 0.0;
    double max_val = 0.0;
    double max_diff = 0.0;

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            for (int c = 0; c < C; ++c) {
                double f = (original(x,y,c));
                double fh = (processed(x,y,c));
                double diff = f - fh;

                sum_diff_sq += diff * diff;
                sum_signal_sq += f * f;
                if (f > max_val) {
                    max_val = f;
                }

                if (abs(diff) > max_diff) {
                    max_diff = abs(diff);
                }
            }
        }
    }



    double MSE = sum_diff_sq / total;
    double PMSE;
    double SNR;
    double PSNR;
    double MD = max_diff;

    if (max_val > 0) {
        PMSE= sum_diff_sq / (total * max_val * max_val);
    }
    else {
        PMSE = 0;
    }

    if (sum_diff_sq > 0) {
        SNR = 10.0 * log10(sum_signal_sq / sum_diff_sq);
        PSNR = 10.0 * log10((max_val * max_val * total)/ sum_diff_sq);
    }
    else {
        PSNR = numeric_limits<double>::infinity();
        SNR = numeric_limits<double>::infinity();
    }





    cout << "MSE = " << MSE << endl;
    cout << "PMSE = " << PMSE << endl;
    cout << "SNR = " << SNR << endl;
    cout << "PSNR = " << PSNR << endl;
    cout<< "MD = " << MD << endl;

}
