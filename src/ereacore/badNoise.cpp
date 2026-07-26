#include <ereacore/badNoise.h>
#include <random>
#include <3dListUtil.h>
#include <iostream>

double badNoise::sample2dNoise(int x, int y) {
    //https://math.stackexchange.com/a/23505
    long double seed = (1<<y)*std::pow(3.0,x);
    rnd.seed(seed);
    return uni(rnd);
}

double badNoise::sample2dNoiseD(double x, double y) {
    double seed = std::pow(7.0,x)*std::pow(3.0,y);
    unsigned long long int seedInt = static_cast<unsigned long long int>(seed);
    rnd.seed(seedInt);
    return uni(rnd);
}

//https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1/creating-simple-2D-noise.html
double badNoise::bilinearInterpolation(double lerpX, double lerpY, double topLeft, double topRight, double bottomLeft, double bottomRight) {
    double topLerp = lerpX*topRight + (1-lerpX)*topLeft;
    double bottomLerp = lerpX*bottomRight + (1-lerpX)*bottomLeft;
    return topLerp*lerpY + (1-lerpY)*bottomLerp;
}

//https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1/creating-simple-2D-noise.html
double badNoise::bilinearNoise(double x, double y) {
    double fx, cx, fy, cy;
    fx = std::floor(x);
    cx = std::ceil(x);
    fy = std::floor(y);
    cy = std::ceil(y);

    double topLeft, topRight, bottomLeft, bottomRight;
    topLeft = sample2dNoiseD(fx,fy);
    topRight = sample2dNoiseD(cx,fy);
    bottomLeft = sample2dNoiseD(fx,cy);
    bottomRight = sample2dNoiseD(cx,cy);

    double lerpX, lerpY;
    lerpX = x - fx;
    lerpY = y - fy;

    return bilinearInterpolation(lerpX,lerpY,topLeft,topRight,bottomLeft,bottomRight);
}

void badNoise::printStuff() {
    double x = std::floor(uni(rnd));
    double y = std::floor(uni(rnd));
    for (double offset : {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0}) {
        std::cout << "l:" << bilinearInterpolation(offset,offset,0.0,0.0,1.0,1.0);
        std::cout << "n:" << bilinearNoise(x+offset,y) << "\n";
    }
}












double badNoise::linear2dNoise9(double x, double y) {
    int xStart = static_cast<int>(x);
    int yStart = static_cast<int>(y);
    double values[9];
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            values[posToIdx(i+1,j+1,0,3)] = sample2dNoise(xStart+i,yStart+j);
        }
    }

    double longestDist = 0;
    double dists[9];
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            double dista = dist(ListUtilVec{x-xStart+i,y-yStart+j,0});
            int idx = posToIdx(i+1,j+1,0,3);

            if (dista > longestDist) {
                longestDist = dista;
            }
            dists[idx] = dista;
        }
    }
    double lerpv[9];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int idx = posToIdx(i,j,0,3);
            lerpv[idx] = 1 - dists[idx]/longestDist;
        }
    }

    double finalValue = 0;
    double valAtStart = values[posToIdx(1,1,0,3)]; //val at xStart,yStart
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int idx = posToIdx(i,j,0,3);
            finalValue += lerpv[idx]*values[idx]+(1-lerpv[idx])*valAtStart;
        }
    }
    finalValue = finalValue/9;

    return finalValue;
}