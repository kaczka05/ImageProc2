//
// Created by damia on 15.11.2025.
//

#include "reyleigh.h"

#include <iostream>
#include <ostream>

void computeReyleigh(cimg_library::CImg<unsigned char>& image, int gmin, int gmax) {
    cimg_library::CImg<unsigned char> outputImage = image;
    float histogram [256]  = {0} ;
    float new_histogram [256] = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    analyseHistogram(histogram,image.width()*image.height());
    float newIntensity = 0;
    for (int i =0 ;i<256;i++) {
        newIntensity = 0;
        for (int j = 0;j<=i;j++) {
            newIntensity += histogram[j];
        }
        newIntensity = newIntensity / (image.height()*image.width());
        newIntensity = 1 - newIntensity;
        newIntensity = log(max((double)newIntensity, 1.0/(image.height()*image.width()+1.0)));
        newIntensity = ((float)(gmax-gmin)/3.0) * sqrt(-2*newIntensity)+gmin;
        newIntensity = min(newIntensity,(float)255);
        new_histogram[i] = newIntensity;
    }
    analyseHistogram(new_histogram,image.width()*image.height());
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            outputImage(x,y,0) = new_histogram[image(x,y, 0)];
        }
    }
    outputImage.save("reyleigh.bmp");
}
void analyseHistogram(float histogram[256],float pixelAmount) {
    float output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    cout << "Mean = " << output << endl;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    cout << "Variance = " << output << endl;
    output = sqrt(output);
    cout << "Standard deviation = " << output << endl;
    cout << "Variation coefficient I = " << output/median << endl;
    float deviation = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),3);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,3);
    cout << "Asymmetry coefficient = " << output << endl;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),4);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,4);
    output = output - 3;
    cout << "Flattening coefficient = " << output << endl;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += pow(histogram[i],2);

    }
    output = output/pow(pixelAmount,2);
    cout << "Variation coefficient II = " << output << endl;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        if (histogram[i] != 0) {
            output += histogram[i]* log2(histogram[i]/pixelAmount);
        }
    }
    output = output/pixelAmount * -1.0;
    cout << "Information source entropy = " << output << endl;
}

void mean(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    float output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    cout << "Mean = " << output << endl;
}
void variance(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    cout << "Variance = " << output << endl;
}
void standardDeviation(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    cout << "Standard deviation = " << output << endl;
}
void variationCoefficient1(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    cout << "Variation coefficient I = " << output/median << endl;
}
void asymmetryCoefficient(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    float deviation = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),3);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,3);
    cout << "Asymmetry coefficient = " << output << endl;
}
void flatteningCoefficient(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    float deviation = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),3);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,3);
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),4);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,4);
    output = output - 3;
    cout << "Flattening coefficient = " << output << endl;
}
void variationCoefficient2(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    float deviation = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),3);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,3);
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),4);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,4);
    output = output - 3;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += pow(histogram[i],2);

    }
    output = output/pow(pixelAmount,2);
    cout << "Variation coefficient II = " << output << endl;
}
void informationSourceEnthropy(cimg_library::CImg<unsigned char>& image,float pixelAmount) {
    float output = 0;
    float histogram [256]  = {0} ;
    for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++ ) {
            histogram[image(x,y,0)] ++;
        }
    }
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i);
    }
    output = output/pixelAmount;
    float median = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),2);
    }
    output = output/pixelAmount;
    output = sqrt(output);
    float deviation = output;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),3);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,3);
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*pow(((i)-median),4);
    }
    output = output/pixelAmount;
    output = output/pow(deviation,4);
    output = output - 3;
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += pow(histogram[i],2);

    }
    output = output/pow(pixelAmount,2);
    output = 0;
    for (int i = 0 ; i < 256 ; i++) {
        output += histogram[i]*(i) * log2(histogram[i]*(i)/pixelAmount);
    }
    output = output/pixelAmount * -1;
    cout << "Information source entropy = " << output << endl;
}

