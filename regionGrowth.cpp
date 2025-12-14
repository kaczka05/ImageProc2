//
// Created by damia on 01.12.2025.
//

#include "regionGrowth.h"

#include <iostream>
#include <ostream>

#include <map>




void regionGrowth(cimg_library::CImg<unsigned char>& image, int seedAmount) {
    ios_base::sync_with_stdio(0);
    auto memRegions = new region[1000];
    multimap<int, coor> queue;
    float varianceFloor = 100.0; // upper boundry of allowed variance
    int imageHeight = image.height();
    int basex, basey;
    int height = image.height();
    static int beenThere[2000][2000] = {0};
    region tempRegion;
    coor tempCoor;
    int key;
    for(int i = 1; i <= seedAmount; i++) {
        basex = rand()%(image.width()-2)+1;
        basey = rand()%(image.height()-2)+1;//przepisz reszte na basex/y, dodaj tablice parametrow dla regionu, kod na dodawanie, kod na merge, insert dodaje w zaleznosci od dystansu sredniej
        cout << basex << " " << basey << '\n';
        tempRegion.pixelsInside.clear();
        tempRegion.regionMean = 0;
        beenThere[basex][basey] = i;
        cout << basex << " " << basey << '\n';
        tempCoor.x = basex;
        tempCoor.y = basey;
        tempCoor.region = i;
        tempCoor.intensity = image(basex,basey,0);
        tempRegion.pixelsInside.push_back(tempCoor);
        tempRegion.regionMean = image(basex,basey,0);
        memRegions[i] = tempRegion;
        cout << basex << " " << basey << '\n';

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
        cout << basex << " " << basey << '\n';

        tempCoor.x = basex-1;
        tempCoor.y = basey;
        tempCoor.region = i;
        tempCoor.intensity = image(basex-1,basey,0);
        key = abs(calculateNewParameter(memRegions[i].regionVariance,memRegions[i].regionMean,memRegions[i].regionSize,tempCoor.intensity,0)-memRegions[i].regionMean);
        queue.insert({key,tempCoor});
        cout << basex << " " << basey << '\n';

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
    region currentRegion;
    boolean passed;
    cout << "entered main loop" <<'\n';
    cout << queue.size() << '\n';
    while (queue.size()>0) {
        currentCoor = queue.begin()->second;
        //cout << '\n';
        //cout << currentCoor.x << " " << currentCoor.y << '\n';
        //cout << currentCoor.region << " " << currentCoor.intensity << '\n';
        //cout << beenThere[currentCoor.x][currentCoor.y] << '\n';
        while(memRegions[currentCoor.region].newOwner != -1) { // partialy universal code to get to the owning region
            currentCoor.region = memRegions[currentCoor.region].newOwner;
        }
        currentRegion = memRegions[currentCoor.region];
        if (beenThere[currentCoor.x][currentCoor.y] != 0)  // check if pixel already owned
        {
            while(memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner != -1) {
                beenThere[currentCoor.x][currentCoor.y] = memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner;
            }
            if (beenThere[currentCoor.x][currentCoor.y] != currentCoor.region) { // check if not expanding to same region
                //region merge code
                //cout << "POTENTIAL MERGE" << '\n';
                reg1 = memRegions[beenThere[currentCoor.x][currentCoor.y]];
                reg2 = memRegions[currentCoor.region];
                f1 = calculateNewRegionParameters(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize,0);
                newSize = reg1.regionSize + reg2.regionSize;
                //cout << "mergo prep" << '\n';
                //(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize))
                if (doesNewParameterFit(reg1,reg2.regionMean) && doesNewParameterFit(reg2,reg1.regionMean)) {
                    if (reg1.regionVariance!= 0 && reg2.regionVariance!= 0) {
                        f2 = calculateNewRegionParameters(reg1.regionMean,reg1.regionVariance,reg1.regionSize,reg2.regionMean,reg2.regionVariance,reg2.regionSize,0);
                        if (doesNewVarianceFit(reg1,f2) && doesNewVarianceFit(reg2,f2) && f2<varianceFloor) {
                            memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner = currentCoor.region;
                            memRegions[currentCoor.region].regionMean = f1;
                            memRegions[currentCoor.region].regionVariance = f2;
                            memRegions[currentCoor.region].regionSize = newSize;
                            //cout << "merged region" << '\n';
                        }
                    }
                    else {
                        memRegions[beenThere[currentCoor.x][currentCoor.y]].newOwner = currentCoor.region;
                        memRegions[currentCoor.region].regionMean = f1;
                        memRegions[currentCoor.region].regionSize = newSize;
                        if (newSize>= 10 && reg2.regionVariance == 0) {
                            memRegions[currentCoor.region].regionVariance = calculateVariance(memRegions[currentCoor.region],memRegions[beenThere[currentCoor.x][currentCoor.y]] );
                        }
                        //cout << "merged region" << '\n';
                    }
                }
            }// alternative means that expansion was planned on the pixel claimed by the same region
        }
        else{ //possible expansion on empty pixel
            //cout << "expansion" << '\n';
            passed = false;
            f1 = calculateNewParameter(currentRegion.regionVariance,currentRegion.regionMean,currentRegion.regionSize,currentCoor.intensity,0);
            //cout << currentRegion.regionMean << " " << currentRegion.regionVariance << " " <<  currentRegion.regionSize << '\n' ;
            //cout << f1 << '\n';
            if (doesNewParameterFit(currentRegion,currentCoor.intensity)) {
                if (currentRegion.regionVariance!= 0) {
                    f2 = calculateNewParameter(currentRegion.regionVariance,currentRegion.regionMean,currentRegion.regionSize+1,currentCoor.intensity,1);
                    if (f2<varianceFloor) {
                        memRegions[currentCoor.region].regionVariance = f2;
                        passed = true;
                    }
                }
                else {
                    passed = true;
                }
            }
            if (passed) {
                //cout << "passed" << '\n';
                memRegions[currentCoor.region].regionMean = f1;
                memRegions[currentCoor.region].pixelsInside.push_back(currentCoor);
                memRegions[currentCoor.region].regionSize += 1;
                tempRegion.newOwner = -2;
                if (currentRegion.regionVariance== 0 && currentRegion.regionSize>=10) {
                    memRegions[currentCoor.region].regionVariance = calculateVariance(memRegions[currentCoor.region], tempRegion);
                }

                basex = currentCoor.x;
                basey = currentCoor.y;
                beenThere[basex][basey] = currentCoor.region;
                if (basex < image.width()-1 ) {
                    if (beenThere[basex+1][basey] != currentCoor.region) {
                        tempCoor.x = basex+1;
                        tempCoor.y = basey;
                        tempCoor.region = currentCoor.region;
                        tempCoor.intensity = image(basex+1,basey,0);
                        key = abs(calculateNewParameter(memRegions[currentCoor.region].regionVariance,memRegions[currentCoor.region].regionMean,memRegions[currentCoor.region].regionSize,tempCoor.intensity,0)-memRegions[currentCoor.region].regionMean);
                        queue.insert({key,tempCoor});
                    }
                }
                if (basey < image.height()-1 ) {
                    if (beenThere[basex][basey+1] != currentCoor.region) {
                        tempCoor.x = basex;
                        tempCoor.y = basey+1;
                        tempCoor.region = currentCoor.region;
                        tempCoor.intensity = image(basex,basey+1,0);
                        key = abs(calculateNewParameter(memRegions[currentCoor.region].regionVariance,memRegions[currentCoor.region].regionMean,memRegions[currentCoor.region].regionSize,tempCoor.intensity,0)-memRegions[currentCoor.region].regionMean);
                        queue.insert({key,tempCoor});
                    }
                }
                if (basex>0 ) {
                    if (beenThere[basex-1][basey] != currentCoor.region) {
                        tempCoor.x = basex-1;
                        tempCoor.y = basey;
                        tempCoor.region = currentCoor.region;
                        tempCoor.intensity = image(basex-1,basey,0);
                        key = abs(calculateNewParameter(memRegions[currentCoor.region].regionVariance,memRegions[currentCoor.region].regionMean,memRegions[currentCoor.region].regionSize,tempCoor.intensity,0)-memRegions[currentCoor.region].regionMean);
                        queue.insert({key,tempCoor});
                    }
                }
                if (basey>0 ) {
                    if (beenThere[basex][basey-1] != currentCoor.region) {
                        tempCoor.x = basex;
                        tempCoor.y = basey-1;
                        tempCoor.region = currentCoor.region;
                        tempCoor.intensity = image(basex,basey-1,0);
                        key = abs(calculateNewParameter(memRegions[currentCoor.region].regionVariance,memRegions[currentCoor.region].regionMean,memRegions[currentCoor.region].regionSize,tempCoor.intensity,0)-memRegions[currentCoor.region].regionMean);
                        queue.insert({key,tempCoor});
                    }
                }
            }
        }
        queue.erase(queue.begin());
        cout << queue.size() << '\n';
    }
    //cout << "finished main loop" << '\n';
    int counter = 0;
    cimg_library::CImg<unsigned char> outputImage = image;
    int regionNumber;
    for (int i=1 ; i<=seedAmount;i++) {
        tempRegion = memRegions[i];
        //cout << tempRegion.regionVariance << '\n';
        regionNumber = i;
        while(memRegions[regionNumber].newOwner != -1) { // partialy universal code to get to the owning region
            regionNumber = memRegions[regionNumber].newOwner;
        }
        for (coor pix : memRegions[i].pixelsInside) {
            outputImage(pix.x,pix.y,0) = regionNumber%2 * 255;
            outputImage(pix.x,pix.y,1) = regionNumber/2 * 127;
            outputImage(pix.x,pix.y,2) = regionNumber/4 * 127;
            counter++;
        }
    }
    outputImage.save("regionGrowth.bmp");
    cout << counter << '\n';
}



boolean doesNewParameterFit(region reg, float newRegionParameter) {
    //Parametrisation up to user
    float meanCriteria = 20;
    float varianceCriteria = 2.35;
    if (reg.regionVariance == 0) {
        if (abs(reg.regionMean-newRegionParameter) < meanCriteria) {
            return 1;
        }
    }
    else {
        if (abs(reg.regionMean-newRegionParameter) < varianceCriteria*sqrt(reg.regionVariance) && abs(reg.regionMean-newRegionParameter) < meanCriteria*2) {
            return 1;
        }
    }
    return false;
}

boolean doesNewVarianceFit(region reg, float newVariance) {
    float varianceCriteria = 15;
    if (abs(newVariance-reg.regionVariance) < varianceCriteria) {
        return 1;
    }
    return false;
}

float calculateNewParameter(float regionVariance,float regionMean,int regionSize, int pixelIntensity, int parameterId) {
    //check if output is in floating point
    if (parameterId == 0) {
        //For intensity criteria
        return (regionMean*regionSize+pixelIntensity)/(regionSize+1);
    }
    else if (parameterId == 1) {
        //For variance criteria
        float newMean = (regionMean * regionSize + pixelIntensity) / (regionSize + 1);
        return ((regionSize*regionVariance + (pixelIntensity - regionMean)*(pixelIntensity-newMean))) / (regionSize+1);
    }
    return -1;
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
    return -1;
}

float calculateVariance(region reg,region reg2) {
    float variance = 0;
    for (int i=0;i<reg.pixelsInside.size();i++) {
        variance += pow(reg.pixelsInside[i].intensity-reg.regionMean,2);
    }
    if(reg2.newOwner != -2) {
        for (int i=0;i<reg2.pixelsInside.size();i++) {
            variance += pow(reg2.pixelsInside[i].intensity-reg.regionMean,2);
        }
    }
    variance /= reg.regionSize;
    cout << "variance" << '\n';
    cout << variance << '\n';
    return variance;
}

