#pragma once

#include <random>
#include <ereacore/chunk.h>

class terrainGenerator {
    std::mt19937 rnd = std::mt19937();
    std::uniform_real_distribution<double> uni = std::uniform_real_distribution();
    double sample2dNoise(int x, int y);
    double linear2dNoise(double x, double y);
    public:
    void generateChunk(Chunk& chunk);
};
