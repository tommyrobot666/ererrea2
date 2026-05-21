#pragma once

#include <FastNoise/FastNoise.h>
#include <random>
#include <ereacore/chunk.h>

class terrainGenerator {
    std::mt19937 rnd = std::mt19937();
    std::uniform_real_distribution<double> uni = std::uniform_real_distribution();
    FastNoise::SmartNode<FastNoise::Simplex> simplex;
    FastNoise::SmartNode<FastNoise::FractalFBm> fractal;
    FastNoise::SmartNode<FastNoise::Remap> remap;
    FastNoise::SmartNode<FastNoise::Remap> debugRemap;
    int seed = 0;
    double sample2dNoise(int x, int y);
    double linear2dNoise9(double x, double y);
    public:
    terrainGenerator();
    void generateChunk(Chunk& chunk);
    int debugTex;
};
