#pragma once

#include <random>

class badNoise {
    std::mt19937 rnd = std::mt19937();
    std::uniform_real_distribution<double> uni = std::uniform_real_distribution();
public:
    double sample2dNoise(int x, int y);
    double sample2dNoiseD(double x, double y);
    double bilinearInterpolation(double lerpX, double lerpY, double topLeft, double topRight, double bottomLeft,
                                 double bottomRight);
    double bilinearNoise(double x, double y);

    double linear2dNoise9(double x, double y);
};
