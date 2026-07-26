#include <glad/glad.h>
#include <FastNoise/FastNoise.h>
#include <random>
#include <iostream>
#include <3dListUtil.h>
#include <ereacore/terrainGenerator.h>
#include <core/gameState.h>

terrainGenerator::terrainGenerator() {
    auto simplex = FastNoise::New<FastNoise::Simplex>();;
    simplex->SetScale(150);
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource(simplex);
    fractal->SetOctaveCount(7);
    fractal->SetGain(.3);
    remap = FastNoise::New<FastNoise::Remap>();
    remap->SetSource(fractal);
    remap->SetFromMin(-1);
    remap->SetFromMax(1);
    remap->SetToMin(0);
    remap->SetToMax(Chunk::LENGTH);

    debugRemap = FastNoise::New<FastNoise::Remap>();
    debugRemap->SetSource(simplex);
    debugRemap->SetFromMin(-1);
    debugRemap->SetFromMax(1);
    debugRemap->SetToMin(0);
    debugRemap->SetToMax(1);
}

void terrainGenerator::debugtex(int debugTex) {
    constexpr int debugSize = 256;
    float debugValues[debugSize*debugSize];
    debugRemap->GenUniformGrid2D(debugValues,0,0,debugSize,debugSize,0.00001,0.00001,seed);
    for (int x = 0; x < debugSize; ++x) {
        std::cout << debugValues[x+8*debugSize] << "\n";
    }
    glBindTexture(GL_TEXTURE_2D, debugTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, debugSize,debugSize, 0, GL_RED, GL_UNSIGNED_BYTE, debugValues);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void terrainGenerator::generateNearbyChunks(std::vector<Chunk>& chunks) {
    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    int maxChunkDistance = 3;
    for (int x = cameraChunkPos.x-maxChunkDistance; x < cameraChunkPos.x+maxChunkDistance; ++x) {
        for (int y = cameraChunkPos.y-maxChunkDistance; y < cameraChunkPos.y+maxChunkDistance; ++y) {
            for (int z = cameraChunkPos.z-maxChunkDistance; z < cameraChunkPos.z+maxChunkDistance; ++z) {
                Chunk* isChunk = Chunk::findChunkOrNone(chunks,x,y,z);
                if (isChunk != nullptr) continue;
                chunks.emplace_back(x,y,z);
                auto& chunk = chunks.back();
                generateChunk(chunk);
            }
        }
    }
}

void terrainGenerator::generateChunk(Chunk &chunk) {
    if (chunk.y >= 1) return;
    if (chunk.y == -1) {
        chunk.fillUnits(0,Chunk::LENGTH-3,0,Chunk::LENGTH,Chunk::LENGTH,Chunk::LENGTH,Unit::DIRT);
        chunk.fillUnits(0,0,0,Chunk::LENGTH,Chunk::LENGTH-3,Chunk::LENGTH,Unit::STONE);
        return;
    }
    if (chunk.y < -1) {
        chunk.fillUnits(0,0,0,Chunk::LENGTH,Chunk::LENGTH,Chunk::LENGTH,Unit::STONE);
        chunk.fillUnits(0,0,0,8,8,8,Unit::ORE);
        return;
    }
    float perlinValues[Chunk::LENGTH*Chunk::LENGTH];
    remap->GenUniformGrid2D(perlinValues,chunk.x*Chunk::LENGTH,chunk.z*Chunk::LENGTH,
        Chunk::LENGTH,Chunk::LENGTH,1,1,seed);

    for (int x = 0; x < Chunk::LENGTH; ++x) {
        for (int z = 0; z < Chunk::LENGTH; ++z) {
            int val = perlinValues[posToIdx(x,z,0,Chunk::LENGTH)];
            int localV = val-chunk.y*Chunk::LENGTH;
            localV = (localV<0?0:(localV>Chunk::LENGTH?Chunk::LENGTH:localV));
            for (int y = 0; y < Chunk::LENGTH; ++y) {
                if (y > localV) continue;

                Unit unit = Unit::DIRT;
                if (localV-y < 3) unit = Unit::GRASS;
                if (y > 10) unit = Unit::STONE;

                chunk.setUnit(x,y,z,unit);
            }
        }
    }
}
