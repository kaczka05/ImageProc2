#include "CImg.h"
#include <iostream>
#include  "funtions.h"
#include "similarity.h"
#include "histogram.h"
#include <cstdlib>

#include "LLFilter.h"
#include "reyleigh.h"
#include "perf.h"
#include "lowpass_filter.h"
#include "morphology.h"
#include "regionGrowth.h"
#include "MorphologicalM4.h"
#include <chrono>

#include "TransFourier.h"


using namespace std;
using namespace cimg_library;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    string file_name, command_name;
    string input, input2;
    bool outputToOriginal;
    //cin >> file_name; do odkomentowania jak bedzie trzeba
    //cin.ignore(2);
    do {
        cout << "Write your command in fromat '[file name] [--command] [--argument=value] ... ' use --help to learn the commands " << '\n';


        cin >> file_name;


        CImg<unsigned char> image(file_name.c_str());
        CImg<unsigned char> orig = image;

        //cin.ignore(3);
        //odkomendowac w kodzie produkcyjnym
        cin >> command_name;
        outputToOriginal = true;



        if (command_name == "brightness" || command_name == "b") {
            cin.ignore(8);
            cin >> input;
            brightness(image, stoi(input));
        }

        else if (command_name == "contrast" || command_name == "c") {
            cin.ignore(9);
            cin >> input;
            contrast(image, stoi(input));
        }

        else if (command_name == "negative" || command_name == "n") {
            negative(image);
        }


        else if (command_name == "horizontal_flip" || command_name == "hf"){

            horizontal_flip(image);
        }

        else if (command_name == "vertical_flip" || command_name == "vf"){

            vertical_flip(image);
        }



        else if (command_name == "diagonal_flip" || command_name == "df") {

            diagonal_flip(image);
            outputToOriginal = false;
        }

        else if (command_name == "change_size" || command_name == "chs") {
            cin.ignore(8);
            cin >> input;
            cin.ignore(9);
            cin >> input2;
            change_size(image, stoi(input), stoi(input2));
        }

        else if (command_name == "adaptive" || command_name == "a") {
            cin.ignore(6);
            cin >> input;
            int minSize = stoi(input);
            cin.ignore(6);
            cin >> input;
            int maxSize = stoi(input);
            adaptive_filter(image, minSize, maxSize);
            CImg<unsigned char> orig("lenac.bmp");
            CImg<unsigned char> image_out("out.bmp");
            compare_similarity(orig, image_out);
            outputToOriginal = false;
        }

        else if (command_name == "min" ) {
            cin.ignore(7);
            cin >> input;
            min_filter(image, stoi(input));
            outputToOriginal = false;
        }

        else if (command_name == "max" ) {
            cin.ignore(7);
            cin >> input;
            max_filter(image, stoi(input));
            outputToOriginal = false;
        }

        else if ( command_name == "test") {
            string file_name1, file_name2;
            cin>>file_name2;

            CImg<unsigned char> test1(file_name.c_str());
            CImg<unsigned char> test2(file_name2.c_str());
            compare_similarity(test1, test2);
            outputToOriginal = false;
        }

        else if (command_name == "histogram" || command_name == "his") {
            outputToOriginal = false;
            create_histogram(image);
        }

        else if ( command_name == "hrayleigh" || command_name == "hr") {
            outputToOriginal = false;
            cin >> input;
            int minSize = stoi(input);
            cin >> input;
            int maxSize = stoi(input);
            computeReyleigh(image, minSize, maxSize);
        }

        else if ( command_name == "cmean" ) {
            outputToOriginal = false;
            mean(image,image.width()*image.height());
        }
        else if ( command_name == "cvariance" ) {
            outputToOriginal = false;
            variance(image,image.width()*image.height());
        }
        else if ( command_name == "cstdev" ) {
            outputToOriginal = false;
            standardDeviation(image,image.width()*image.height());
        }
        else if ( command_name == "cvarcoi" ) {
            outputToOriginal = false;
            variationCoefficient1(image,image.width()*image.height());
        }
        else if ( command_name == "casyco" ) {
            outputToOriginal = false;
            asymmetryCoefficient(image,image.width()*image.height());
        }
        else if ( command_name == "cvarcoii" ) {
            outputToOriginal = false;
            variationCoefficient2(image,image.width()*image.height());
        }
        else if ( command_name == "centropy" ) {
            outputToOriginal = false;
            informationSourceEnthropy(image,image.width()*image.height());
        }
        else if (command_name == "lowpass" || command_name == "lp") {
            //cin.ignore(8);
            cin >> input;
            lowpass_filter_universal(image, stoi(input));
            outputToOriginal = true;
        }

        else if (command_name == "lowpass_opt" || command_name == "lpo") {
            //cin.ignore(12);
            cin >> input;
            lowpass_filter_optimized(image, stoi(input));
            outputToOriginal = true;
        }
        else if (command_name == "oll") {

            double k;
            cin>>k;
            ll_filter(image, k);
        }

        else if (command_name == "report") {
            perf::report();
            outputToOriginal = false;
        }
        else if (command_name == "regionGrowth" || command_name == "rg") {
            cin >> input;
            outputToOriginal = false;
            regionGrowth(image, stoi(input));
        }

        else if (command_name == "dilate" || command_name == "dilation") {
            std::string type; int size;
            cin >> type >> size;
            morphology::StructuringElement se = morphology::makeStructuringElement(type, size);
            image = morphology::dilate(image, se);
        }


        else if (command_name == "erode" || command_name == "erosion") {
            std::string type; int size;
            cin >> type >> size;
            morphology::StructuringElement se = morphology::makeStructuringElement(type, size);
            image = morphology::erode(image, se);
        }


        else if (command_name == "open" || command_name == "opening") {
            std::string type; int size;
            cin >> type >> size;
            morphology::StructuringElement se = morphology::makeStructuringElement(type, size);
            image = morphology::opening(image, se);
        }


        else if (command_name == "close" || command_name == "closing") {
            std::string type; int size;
            cin >> type >> size;
            morphology::StructuringElement se = morphology::makeStructuringElement(type, size);
            image = morphology::closing(image, se);
        }


        else if (command_name == "hmt") {
            std::string pattern;
            cin >> pattern;
            if (pattern == "endpoint") {
                image = morphology::hitAndMissEndpoint(image);
            }
        }

        else if (command_name == "stfn") {
            outputToOriginal = false;
            spatialToFreq(image);
        }

        else if (command_name == "ftsn") {
            outputToOriginal = false;
            //freqToSpatial(image);
        }

        else if (command_name == "stff") {
            outputToOriginal = false;
            spatialToFreqFast(image);
        }

        else if (command_name == "ftsf") {
            outputToOriginal = false;
            //spatialToFreqFast(image);
        }

else if (command_name == "m4" || command_name == "hmt_m4") {


    std::string rest;
    std::getline(std::cin, rest);
    std::istringstream iss(rest);

    std::vector<std::string> args;
    std::string tok;
    while (iss >> tok) args.push_back(tok);

    bool useAlt = false;
    int threshold_value = 128;

    for (auto &t : args) {
        std::string tl = t;
        for (auto &ch : tl) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        if (tl == "alt" || tl == "alternate") {
            useAlt = true;
            continue;
        }
        try {
            threshold_value = std::stoi(t);
        } catch (...) {}
    }

    //grayscale
    CImg<unsigned char> gray(image.width(), image.height(), 1, 1, 0);
    if (image.spectrum() == 1) {
        gray = image;
    } else {
        cimg_forXY(image, x, y) {
            int sum = 0;
            for (int c = 0; c < image.spectrum(); ++c)
                sum += image(x,y,0,c);
            gray(x,y) = static_cast<unsigned char>(sum / image.spectrum());
        }
    }


    CImg<unsigned char> bin = gray;
    bin.threshold(threshold_value);

    using SE = MorphologicalM4::StructuringElement;
    std::vector<SE> elements;

    auto start = std::chrono::high_resolution_clock::now();

    if (!useAlt) {

        SE B1; B1.B1 = {{+1,0}}; B1.B2 = {}; elements.push_back(B1);
        SE B2; B2.B1 = {{0,+1}}; B2.B2 = {}; elements.push_back(B2);
        SE B3; B3.B1 = {{-1,-1},{0,-1},{+1,-1},{-1,0},{+1,0},{-1,+1},{0,+1},{+1,+1}}; B3.B2 = {}; elements.push_back(B3);
        SE B4; B4.B1 = {{0,-1},{-1,0},{+1,0},{0,+1}}; B4.B2 = {}; elements.push_back(B4);
    } else {

        SE P1; P1.B1 = {{-1,-1},{-1,0},{-1,1}}; P1.B2 = {{0,0}}; elements.push_back(P1);
        SE P2; P2.B1 = {{-1,-1},{0,-1},{1,-1}};  P2.B2 = {{0,0}}; elements.push_back(P2);
        SE P3; P3.B1 = {{1,-1},{1,0},{1,1}};     P3.B2 = {{0,0}}; elements.push_back(P3);
        SE P4; P4.B1 = {{-1,1},{0,1},{1,1}};     P4.B2 = {{0,0}}; elements.push_back(P4);
    }

    // union
    CImg<unsigned char> H(bin.width(), bin.height(), 1, 1, 0);

    for (const auto &B : elements) {
        CImg<unsigned char> hits = MorphologicalM4::hitOrMiss(bin, B);
        cimg_forXY(H, x, y) {
            if (hits(x,y)) H(x,y) = 255;
        }
    }

    H.save_bmp("out.bmp");
    image = H;
    outputToOriginal = false;
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Processing time: " << ms << " ms\n";
}



else if (command_name == "flp") {
    double d; cin >> d;
    freqLowPass(image, d);
}
else if (command_name == "fhp") {
    double d; cin >> d;
    freqHighPass(image, d);
}
else if (command_name == "fbp") {
    double d1, d2; cin >> d1 >> d2;
    freqBandPass(image, d1, d2);
}
else if (command_name == "fbs") {
    double d1, d2; cin >> d1 >> d2;
    freqBandStop(image, d1, d2);
}
else if (command_name == "fdir") {
    double cutoff, dir;
    cin >> cutoff >> dir;
    freqDirectionalHP(image, cutoff, dir);
}
else if (command_name == "fphase") {
    int k, l; cin >> k >> l;
    freqPhaseModify(image, k, l);
}







        else if (file_name == "help" || file_name == "h" || file_name == "--help") {
            cout << "\n=== AVAILABLE COMMANDS ===\n";
            cout << "Input format: [filename] [--command] [--argument=value] ...\n\n";

            cout << "General:\n";
            cout << "  help | h                     : Show this help message.\n";
            cout << "  quit | q                     : Exit the program.\n\n";

            cout << "Image operations:\n";
            cout << "  brightness | b value<int>    : Adjust brightness by given value.\n";
            cout << "                                 Positive = brighter, Negative = darker.\n";
            cout << "  contrast | c factor<float>         : Change contrast by a factor.\n";
            cout << "                                 Example: 1.2 = increase, 0.8 = decrease.\n";
            cout << "  negative | n                 : Invert image colors.\n";
            cout << "  horizontal_flip | hf         : Flip the image horizontally.\n";
            cout << "  vertical_flip | vf           : Flip the image vertically.\n";
            cout << "  diagonal_flip | df           : Flip along the main diagonal.\n";
            cout << "  enlarge | en <width> <height>: Resize image to new dimensions.\n";
            cout << "  adaptive | a <min> <max>     : Apply adaptive filter with min/max window size.\n";
            cout << "  min <size>                   : Apply minimum filter with given window size. \n";
            cout << "  max <size>                   : Apply maximum filter with given window size. \n";
            outputToOriginal = false;
        }

        //image = outputImage;
        //wypisywac output image w przypadku oczyszczania
        if (outputToOriginal) {
            image.save_bmp("out.bmp");
        }


    }while (command_name != "quit" && command_name != "q");

    return 0;
}