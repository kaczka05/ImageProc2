#include "morphology.h"
#include <algorithm>

namespace morphology {
    static inline bool isForeground(unsigned char v) {
        return v > 0;
    }



    StructuringElement makeStructuringElement(
        const std::string& type,
        int size
    ) {
        StructuringElement se;

        if (type == "square") {
            for (int dy = -size; dy <= size; ++dy)
                for (int dx = -size; dx <= size; ++dx)
                    se.emplace_back(dx, dy);
        }
        else if (type == "cross") {
            for (int d = -size; d <= size; ++d) {
                se.emplace_back(d, 0);
                se.emplace_back(0, d);
            }
            std::sort(se.begin(), se.end());
            se.erase(std::unique(se.begin(), se.end()), se.end());
        }
        else {
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    se.emplace_back(dx, dy);
        }

        return se;
    }

    CImg<unsigned char> dilate(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    ) {
        CImg<unsigned char> out(src.width(), src.height(), 1, 1, 0);

        for (int y = 0; y < src.height(); ++y)
            for (int x = 0; x < src.width(); ++x) {
                bool hit = false;
                for (const auto& o : se) {
                    int sx = x - o.first;
                    int sy = y - o.second;
                    if (sx < 0 || sy < 0 ||
                        sx >= src.width() || sy >= src.height())
                        continue;

                    if (isForeground(src(sx, sy))) {
                        hit = true;
                        break;
                    }
                }
                out(x,y) = hit ? 255 : 0;
            }
        return out;
    }

    CImg<unsigned char> erode(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    ) {
        CImg<unsigned char> out(src.width(), src.height(), 1, 1, 0);

        for (int y = 0; y < src.height(); ++y)
            for (int x = 0; x < src.width(); ++x) {
                bool ok = true;
                for (const auto& o : se) {
                    int sx = x + o.first;
                    int sy = y + o.second;
                    if (sx < 0 || sy < 0 ||
                        sx >= src.width() || sy >= src.height() ||
                        !isForeground(src(sx, sy))) {
                        ok = false;
                        break;
                        }
                }
                out(x,y) = ok ? 255 : 0;
            }
        return out;
    }

    CImg<unsigned char> opening(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    ) {
        return dilate(erode(src, se), se);
    }

    CImg<unsigned char> closing(
        const CImg<unsigned char>& src,
        const StructuringElement& se
    ) {
        return erode(dilate(src, se), se);
    }

    CImg<unsigned char> hitAndMissEndpoint(
        const CImg<unsigned char>& src
    ) {
        CImg<unsigned char> out(src.width(), src.height(), 1, 1, 0);

        for (int y = 1; y < src.height() - 1; ++y)
            for (int x = 1; x < src.width() - 1; ++x) {

                if (!isForeground(src(x,y))) continue;

                int fgCount = 0;
                int lastDx = 0, lastDy = 0;

                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (isForeground(src(x+dx,y+dy))) {
                            fgCount++;
                            lastDx = dx;
                            lastDy = dy;
                        }
                    }

                if (fgCount == 1)
                    out(x,y) = 255;
            }

        return out;
    }




    CImg<unsigned char> hitAndMissCustom(
     const CImg<unsigned char>& src,
     const std::vector<int>& mask3x3
 ) {
        CImg<unsigned char> out(src.width(), src.height(), 1, 1, 0);

        if (mask3x3.size() != 9) return out;

        for (int y = 1; y < src.height() - 1; ++y) {
            for (int x = 1; x < src.width() - 1; ++x) {

                bool match = true;
                int idx = 0;

                for (int dy = -1; dy <= 1 && match; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {

                        int m = mask3x3[idx++];

                        // -1 = don't care
                        if (m == -1) continue;

                        int p = isForeground(src(x + dx, y + dy)) ? 1 : 0;


                        if (p != m) {
                            match = false;
                            break;
                        }
                    }
                }

                if (match)
                    out(x, y) = 255;
            }
        }

        return out;
    }




    CImg<unsigned char> m4Iterative(const CImg<unsigned char>& A) {

        std::vector<std::vector<int>> masks_2;
        masks_2.push_back({ 1,  1,  1,
                           -1,  0, -1,
                           -1, -1, -1 }); // top
        masks_2.push_back({ 1, -1, -1,
                            1,  0, -1,
                            1, -1, -1 }); // left
        masks_2.push_back({ -1, -1,  1,
                            -1,  0,  1,
                            -1, -1,  1 }); // right
        masks_2.push_back({ -1, -1, -1,
                            -1,  0, -1,
                             1,  1,  1 }); // bottom


        CImg<unsigned char> H(A.width(), A.height(), 1, 1, 0);


        for (const auto& mask : masks_2) {

            CImg<unsigned char> X = A;
            CImg<unsigned char> prev(X.width(), X.height(), 1, 1, 0);


            do {
                prev = X;

                CImg<unsigned char> hits = hitAndMissCustom(prev, mask);

                CImg<unsigned char> newX = prev;  // preserve monotonicity

                cimg_forXY(newX, x, y) {
                    if (hits(x, y) == 255)
                        newX(x, y) = 255;
                }

                X = std::move(newX);

            } while (!(X == prev));



            cimg_forXY(H, x, y) {
                if (X(x, y) == 255) H(x, y) = 255;
            }
        }

        return H;
    }

}
