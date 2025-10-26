#include "CImg.h"
#include <iostream>
#include  "funtions.h"
#include "similarity.h"
#include <cstdlib>


using namespace std;
using namespace cimg_library;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    string file_name, command_name;
    string input, input2;
    //cin >> file_name; do odkomentowania jak bedzie trzeba
    //cin.ignore(2);
    do {
        cout << "Write your command in fromat '[file name] [--command] [--argument=value] ... ' use --help to learn the commands " << '\n';


        cin >> file_name;


        CImg<unsigned char> image(file_name.c_str());
        CImg<unsigned char> orig = image;

        cin.ignore(3);
        cin >> command_name;



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

            CImg<unsigned char> image_out("out.bmp");
            compare_similarity(image_out, orig);
        }

        else if (command_name == "min" ) {
            cin.ignore(7);
            cin >> input;
            min_filter(image, stoi(input));
        }

        else if (command_name == "max" ) {
            cin.ignore(7);
            cin >> input;
            max_filter(image, stoi(input));
        }

        else if ( command_name == "test") {
            string file_name1, file_name2;
            cin>>file_name2;

            CImg<unsigned char> test1(file_name.c_str());
            CImg<unsigned char> test2(file_name2.c_str());
            compare_similarity(test1, test2);
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
        }

        //image = outputImage;
        //wypisywac output image w przypadku oczyszczania
        if (command_name != "adaptive" && command_name != "a" && command_name != "diagonal_flip" && command_name != "df" && command_name != "min" && command_name != "max") {
            image.save_bmp("out.bmp"); // save the modified image to a file
        }


    }while (command_name != "quit" && command_name != "q");

    return 0;
}