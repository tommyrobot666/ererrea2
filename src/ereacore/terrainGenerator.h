#pragma once

#include <FastNoise/FastNoise.h>
#include <ereacore/chunk.h>

class terrainGenerator {
    FastNoise::SmartNode<FastNoise::Simplex> simplex;
    FastNoise::SmartNode<FastNoise::FractalFBm> fractal;
    FastNoise::SmartNode<FastNoise::Remap> remap;
    FastNoise::SmartNode<FastNoise::Remap> debugRemap;
    public:
    int seed = 0;
    terrainGenerator();
    void debugtex(int debugTex);
    void generateNearbyChunks(std::vector<Chunk>& chunks);
    void generateChunk(Chunk& chunk);
};
