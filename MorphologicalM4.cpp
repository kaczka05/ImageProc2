#include "MorphologicalM4.h"

using namespace cimg_library;

bool MorphologicalM4::inBounds(int x, int y, int w, int h) {
    return x >= 0 && y >= 0 && x < w && y < h;
}

// ------------------------------------------------------------
// Hit-or-Miss Transform
// A ⊗ B = (A ⊖ B1) ∩ (Ac ⊖ B2)
// ------------------------------------------------------------
CImg<unsigned char>
MorphologicalM4::hitOrMiss(const CImg<unsigned char>& A,
                           const StructuringElement& B)
{
    int W = A.width();
    int H = A.height();

    CImg<unsigned char> result(W, H, 1, 1, 0);

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {

            bool ok = true;

            // Erosion of A by B1
            for (const auto& p : B.B1) {
                int nx = x + p.first;
                int ny = y + p.second;
                if (!inBounds(nx, ny, W, H) || A(nx, ny) == 0) {
                    ok = false;
                    break;
                }
            }

            if (!ok) continue;

            // Erosion of Ac by B2
            for (const auto& p : B.B2) {
                int nx = x + p.first;
                int ny = y + p.second;
                if (!inBounds(nx, ny, W, H) || A(nx, ny) != 0) {
                    ok = false;
                    break;
                }
            }

            if (ok)
                result(x, y) = 255;
        }
    }

    return result;
}

// ------------------------------------------------------------
// Iterative rule:
// X0 = A
// Xk = (Xk-1 ⊗ B) ∪ A
// until convergence
// ------------------------------------------------------------
CImg<unsigned char>
MorphologicalM4::iterate(const CImg<unsigned char>& A,
                         const StructuringElement& B)
{
    CImg<unsigned char> X = A;
    CImg<unsigned char> prev;

    do {
        prev = X;
        CImg<unsigned char> h = hitOrMiss(X, B);

        cimg_forXY(X, x, y) {
            if (h(x,y) || A(x,y))
                X(x,y) = 255;
            else
                X(x,y) = 0;
        }

    } while (!(X == prev));

    return X;
}

// ------------------------------------------------------------
// Final result:
// H(A) = D1 ∪ D2 ∪ D3 ∪ D4
// ------------------------------------------------------------
CImg<unsigned char>
MorphologicalM4::computeH(const CImg<unsigned char>& A,
                          const std::vector<StructuringElement>& elements)
{
    int W = A.width();
    int H = A.height();

    CImg<unsigned char> result(W, H, 1, 1, 0);

    for (const auto& B : elements) {
        CImg<unsigned char> D = iterate(A, B);
        cimg_forXY(result, x, y) {
            if (D(x,y))
                result(x,y) = 255;
        }
    }

    return result;
}
