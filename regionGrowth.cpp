//
// Created by damia on 01.12.2025.
//

#include "regionGrowth.h"

#include <iostream>
#include <ostream>

#include <map>




void regionGrowth(cimg_library::CImg<unsigned char>& image, int seedAmount) {
    auto memRegions = new region[1000000];
    multimap<int, coor> queue;
    int imageHeight = image.height();
    int basex, basey;
    auto beenThere = new int[1000000][1000000];
    region tempRegion;
    coor tempCoor;
    int key;
    
    beenThere = {0};
    for(int i = 1; i <= seedAmount; i++) {
        basex = rand()%(image.width()-2)+1;
        basey = rand()%(image.height()-2)+1;//przepisz reszte na basex/y, dodaj tablice parametrow dla regionu, kod na dodawanie, kod na merge, insert dodaje w zaleznosci od dystansu sredniej
        cout << basex << " " << basey << endl;
        beenThere[basex][basey] = i;
        tempCoor.x = basex;
        tempCoor.y = basey;
        tempCoor.region = i;
        tempCoor.intensity = image(basex,basey,0);
        tempRegion.pixelsInside.push_back(tempCoor);
        tempRegion.regionMean = image(basex,basey,0);
        memRegions[i] = tempRegion;

        tempCoor.x = basex+1;
        tempCoor.y = basey;
        tempCoor.region = i;
        tempCoor.intensity = image(basex+1,basey,0);
        key = abs(calculateNewParameter(memRegions[i].regionVariance,memRegions[i].regionMean,memRegions[i].regionSize,tempCoor.intensity,0)-memRegions[i].regionMean);
        queue.insert({key,tempCoor});
        tempCoor.x = basex;
        tempCoor.y = basey+1;
        tempCoor.region = i;
        tempCoor.intensity = image(basex,basey+1,0);
        key = abs(calculateNewParameter(memRegions[i].regionVariance,memRegions[i].regionMean,memRegions[i].regionSize,tempCoor.intensity,0)-memRegions[i].regionMean);
        queue.insert({key,tempCoor});
        tempCoor.x = basex-1;
        tempCoor.y = basey;
        tempCoor.region = i;
        tempCoor.intensity = image(basex-1,basey,0);
        key = abs(calculateNewParameter(memRegions[i].regionVariance,memRegions[i].regionMean,memRegions[i].regionSize,tempCoor.intensity,0)-memRegions[i].regionMean);
        queue.insert({key,tempCoor});
        tempCoor.x = basex;
        tempCoor.y = basey-1;
        tempCoor.region = i;
        tempCoor.intensity = image(basex,basey-1,0);
        key = abs(calculateNewParameter(memRegions[i].regionVariance,memRegions[i].regionMean,memRegions[i].regionSize,tempCoor.intensity,0)-memRegions[i].regionMean);
        queue.insert({key,tempCoor});
    }
    //najpierw testujemy czy nie jest zajęty, jeśli tak, sprawdzamy merge, potem sprawdzamy czy pasuje, jeśli nic to zostawiamy
    float f1,f2;
    coor currentCoor;
    region reg1, reg2;
    int newSize;
    while (queue.size()>0) {
        currentCoor = queue.begin()->second;
        while(memRegions[currentCoor.region].newOwner != -1) { // partialy universal code to get to the owning region
            currentCoor.region = memRegions[currentCoor.region].newOwner;
        }
        if (beenThere[currentCoor.x][currentCoor.y] != 0)  // check if pixel already owned
            while(memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner != -1) {
                beenThere[currentCoor.x][currentCoor.y] = memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner;
            }
            if (beenThere[currentCoor.x][currentCoor.y] != currentCoor.region) { // check if not expanding to same region
                //region merge code
                reg1 = memRegions[beenThere[currentCoor.x][currentCoor.y]];
                reg2 = memRegions[currentCoor.region];
                f1 = calculateNewRegionParameters(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize,0);
                newSize = reg1.regionSize + reg2.regionSize;
                //(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize))
                if (doesNewParameterFit(reg1.regionMean,f1,0,newSize)) {
                    if (reg1.regionVariance!= 0 && reg2.regionVariance!= 0) {
                        f2 = calculateNewRegionParameters(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize,0);
                        if (doesNewParameterFit(reg1.regionVariance,f2,1,newSize)) {
                            memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner = currentCoor.region;
                            memRegions[currentCoor.region].regionMean = f1;
                            memRegions[currentCoor.region].regionVariance = f2;
                            memRegions[currentCoor.region].regionSize = newSize;

                        }
                    }
                    else {
                        memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner = currentCoor.region;
                        memRegions[currentCoor.region].regionMean = f1;
                        memRegions[currentCoor.region].regionSize = newSize;
                        if (newSize>= 10) {
                            memRegions[currentCoor.region].regionVariance = calculateVariance(memRegions[currentCoor.region]);
                        }
                    }
                }
            }
            // alternative means that expansion was planned on the pixel claimed by the same region
        }
        else { //possible expansion on empty pixel
            
           
        }
    }

}

float calculateNewParameter(float regionVariance,float regionMean,int regionSize, int pixelIntensity, int parameterId) {
    //check if output is in floating point
    if (parameterId == 0) {
        //For intensity criteria
        return regionMean*regionSize+pixelIntensity/(regionSize+1);
    }
    else if (parameterId == 1) {
        //For variance criteria
        int newMean = (regionMean+pixelIntensity)/(regionSize+1);
        return (regionSize*regionVariance + (pixelIntensity - regionMean)*(pixelIntensity-newMean)) / (regionSize+1);
    }


}

boolean doesNewParameterFit(float originalRegionParameter, float newRegionParameter, int parameterId,int newSize) {
    //Parametrisation up to user
    if (abs(originalRegionParameter-newRegionParameter) <= 10 && parameterId == 0) {
        return true;
    }
    else if (abs(originalRegionParameter-newRegionParameter) <= 10 && parameterId == 1) {
        return true;
    }
    return false;
}

float calculateNewRegionParameters(float firstRegionMean, float firstRegionVariance, int firstRegionSize, float secondRegionMean, float secondRegionVariance, int secondRegionSize,int parameterId) {
    float newMean = (firstRegionMean*firstRegionSize + secondRegionMean*secondRegionSize) / (firstRegionSize+secondRegionSize);
    if (parameterId == 0) {
        return newMean;
    }
    float newVariance = (firstRegionSize*(firstRegionVariance+pow(firstRegionMean-newMean,2)) + secondRegionSize*(secondRegionVariance+pow(secondRegionMean-newMean,2)))/(firstRegionSize+secondRegionSize);
    if (parameterId == 1) {
        return newVariance;
    }
}

float calculateVariance(region reg) {
    float variance = 0;
    for (int i=0;i<reg.regionSize;i++) {
        variance += pow(reg.pixelsInside[i].intensity-reg.regionMean,2);
    }
    variance /= reg.regionSize;
    return variance;
}