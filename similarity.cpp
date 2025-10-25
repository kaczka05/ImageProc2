#include "CImg.h"
#include "ostream"
#include "iostream"


using namespace std;
using namespace cimg_library;

void compute_similarity(const CImg<unsigned char>& original, const CImg<unsigned char>& processed) {
    if (original.width() != processed.width() ||
        original.height() != processed.height() ||
        original.spectrum() != processed.spectrum()) {
        cout << "Error: Images must have same dimensions and channels.\n";

        }

    int W = original.width();
    int H = original.height();
    int C = original.spectrum();
    long long total = static_cast<long long>(W) * H * C;

    double sum_diff_sq = 0.0;
    double sum_signal_sq = 0.0;
    double max_val = 0.0;
    double max_diff = 0.0;

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            for (int c = 0; c < C; ++c) {
                double f = static_cast<double>(original(x,y,c));
                double fh = static_cast<double>(processed(x,y,c));
                double diff = f - fh;

                sum_diff_sq += diff * diff;
                sum_signal_sq += f * f;
                if (f > max_val) max_val = f;
                if (std::abs(diff) > max_diff) max_diff = std::abs(diff);
            }
        }
    }

    double MSE = sum_diff_sq / total;
    double PMSE = (max_val > 0) ? (sum_diff_sq / (total * max_val * max_val)) : 0;
    double SNR = (sum_diff_sq > 0) ? 10.0 * log10(sum_signal_sq / sum_diff_sq) : std::numeric_limits<double>::infinity();
    double PSNR = (sum_diff_sq > 0) ? 10.0 * log10((max_val * max_val * total) / sum_diff_sq) : std::numeric_limits<double>::infinity();




    cout << "MSE = " << MSE << endl;
    cout << "PMSE = " << PMSE << endl;
    cout << "SNR = " << SNR << endl;
    cout << "PSNR = " << PSNR << endl;

}
