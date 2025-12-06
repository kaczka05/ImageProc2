//
// Created by damia on 01.12.2025.
//

#include "regionGrowth.h"

#include <iostream>
#include <ostream>

#include <map>

struct coor{
    int x,y;
};

void regionGrowth(cimg_library::CImg<unsigned char>& image, int seedAmount) {
    auto seedPoints = new coor[seedAmount];
    multimap<int, coor> queue;
    int imageHeight = image.height();
    int basex, basey;
    auto beenThere = new bool[1000000][1000000];
    for(int i = 0; i < seedAmount; i++) {
        basex = rand()%image.width();
        basey = rand()%image.height();//przepisz reszte na basex/y, dodaj tablice parametrow dla regionu, kod na dodawanie, kod na merge, insert dodaje w zaleznosci od dystansu sredniej
        cout << seedPoints[i].x << " " << seedPoints[i].y << endl;
        beenThere[seedPoints[i].x][seedPoints[i].y] = true;

        queue.insert({image(seedPoints[i].x+1,seedPoints[i].y+1,0),seedPoints[i]});
    }


}