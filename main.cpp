#include "CImg.h"
#include <iostream>
#include <set>
#include  "funtions.h"
#include "similarity.h"


using namespace std;
using namespace cimg_library;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    CImg<unsigned char> image("lena_impulse3.bmp");
    string file_name, command_name;
    int Iargument;
    float Fargument;
    cout << "Hello, write your command in fromat '[file name] [--command] [--argument=value] ... ' use --help to learn the commands " << '\n';
    //cin >> file_name; do odkomentowania jak bedzie trzeba
    //cin.ignore(3);
    cin >> command_name;
    if (command_name == "brightness" || command_name == "b") {
        //cin.ignore(3);

        cin >> Iargument;
        brightness(image, Iargument);
    }

    else if (command_name == "contrast" || command_name == "c") {
        //cin.ignore(3);
        cin >> Fargument;
        contrast(image, Fargument);
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

    else if (command_name == "enlarge" || command_name == "en") {
    int newW, newH;
    cin >> newW >> newH;
    change_size(image, newW, newH);
    }

    else if (command_name == "adaptive" || command_name == "a") {
        cin >> Iargument;
        int minSize = Iargument;
        cin >> Iargument;
        int maxSize = Iargument;
        adaptive_filter(image, minSize, maxSize);
    }

    //image = outputImage;
    //wypisywac output image w przypadku oczyszczania
    if (command_name != "adaptive" && command_name != "a" && command_name != "diagonal_flip" && command_name != "df") {
        image.save_bmp("out.bmp"); // save the modified image to a file
    }
    return 0;
}