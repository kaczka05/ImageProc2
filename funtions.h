//
// Created by HPOMEN on 20.10.2025.
//

#ifndef FUNTIONS_H
#define FUNTIONS_H



#include "CImg.h"
using namespace cimg_library;

void brightness(CImg<unsigned char>& img, int Iargument);
void contrast(CImg<unsigned char>& img, int Iargument);
void negative(CImg<unsigned char>& img);
void horizontal_flip(CImg<unsigned char>& img);
void vertical_flip(CImg<unsigned char>& img);
void diagonal_flip(CImg<unsigned char>& img);
void change_size(CImg<unsigned char>& img, int newW, int newH);
void adaptive_filter(CImg<unsigned char>& img, int minSize, int maxSize);

#endif //FUNTIONS_H
