#pragma once

#include <random>

class badNoise {
    std::mt19937 rnd = std::mt19937();
    std::uniform_real_distribution<double> uni = std::uniform_real_distribution();
public:
    double sample2dNoise(int x, int y);
    double linear2dNoise9(double x, double y);
};
