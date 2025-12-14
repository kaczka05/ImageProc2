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



        else if (command_name == "region_grow" || command_name == "rg") {
            int seedThreshold, mergeThreshold, connectivity;
            cin >> seedThreshold >> mergeThreshold >> connectivity;
            image = region_growing::regionGrowAuto(image, seedThreshold, mergeThreshold, connectivity);
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
            image.save_bmp("out.bmp"); // save the modified image to a file
        }


    }while (command_name != "quit" && command_name != "q");

    return 0;
}