//
// Created by HPOMEN on 20.10.2025.
//
#include "similarity.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

using namespace cimg_library;

static void check_compatible(const CImg<unsigned char>& a, const CImg<unsigned char>& b) {
    if (a.width() != b.width() || a.height() != b.height() || a.spectrum() != b.spectrum()) {
        throw std::runtime_error("Images must have same width, height and number of channels.");
    }
}

SimilarityMetrics compute_similarity(const CImg<unsigned char>& original,
                                     const CImg<unsigned char>& processed) {
    check_compatible(original, processed);

    int W = original.width();
    int H = original.height();
    int C = original.spectrum();

    long long total_values = static_cast<long long>(W) * H * C;

    double sum_diff_sq = 0.0;
    double sum_signal_sq = 0.0;
    double max_val = 0.0;
    double max_diff = 0.0;

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            for (int c = 0; c < C; ++c) {
                double f = static_cast<double>( original(x,y,c) );
                double fh = static_cast<double>( processed(x,y,c) );
                double diff = f - fh;
                sum_diff_sq += diff * diff;
                sum_signal_sq += f * f;
                if (f > max_val) max_val = f;
                if (std::abs(diff) > max_diff) max_diff = std::abs(diff);
            }
        }
    }

    // MSE: average squared error across all scalar values (pixels * channels)
    double MSE = (total_values > 0) ? (sum_diff_sq / static_cast<double>(total_values)) : 0.0;

    // PMSE: MSE normalized by (max pixel value)^2
    double PMSE;
    if (max_val <= 0.0) {
        PMSE = std::numeric_limits<double>::infinity(); // degenerate: all-zero image
    } else {
        PMSE = MSE / (max_val * max_val);
    }

    // SNR (dB): 10 * log10( sum_signal_sq / sum_diff_sq )
    double SNR;
    if (sum_diff_sq == 0.0) {
        SNR = std::numeric_limits<double>::infinity(); // perfect match -> infinite SNR
    } else if (sum_signal_sq == 0.0) {
        SNR = -std::numeric_limits<double>::infinity(); // zero signal, but nonzero noise
    } else {
        SNR = 10.0 * std::log10( sum_signal_sq / sum_diff_sq );
    }

    // PSNR (dB): 10 * log10( (max_val^2) / MSE )
    double PSNR;
    if (MSE == 0.0) {
        PSNR = std::numeric_limits<double>::infinity();
    } else if (max_val <= 0.0) {
        PSNR = -std::numeric_limits<double>::infinity();
    } else {
        PSNR = 10.0 * std::log10( (max_val * max_val) / MSE );
    }

    SimilarityMetrics metrics;
    metrics.MSE  = MSE;
    metrics.PMSE = PMSE;
    metrics.SNR  = SNR;
    metrics.PSNR = PSNR;
    metrics.MD   = max_diff;
    return metrics;
}
