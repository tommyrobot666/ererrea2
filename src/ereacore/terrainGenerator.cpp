#include <glad/glad.h>
#include <FastNoise/FastNoise.h>
#include <random>
#include <3dListUtil.h>
#include <ereacore/terrainGenerator.h>
#include <core/gameState.h>

double terrainGenerator::sample2dNoise(int x, int y) {
    //https://math.stackexchange.com/a/23505
    long double seed = (1<<y)*std::pow(3.0,x);
    rnd.seed(seed);
    return uni(rnd);
}

double terrainGenerator::linear2dNoise9(double x, double y) {
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

terrainGenerator::terrainGenerator() {
    simplex = FastNoise::New<FastNoise::Simplex>();
    simplex->SetScale(1);
    fractal = FastNoise::New<FastNoise::FractalFBm>();
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
    debugRemap->SetSource(fractal);
    debugRemap->SetFromMin(-1);
    debugRemap->SetFromMax(1);
    debugRemap->SetToMin(0);
    debugRemap->SetToMax(1);
}

void terrainGenerator::debugtex() {
    constexpr int debugSize = 256;
    float debugValues[debugSize*debugSize];
    // debugRemap->GenUniformGrid2D(debugValues,0,0,debugSize,debugSize,1,1,seed);
    for (int x = 0; x < debugSize; ++x) {
        for (int z = 0; z < debugSize; ++z) {
            debugValues[posToIdx(x,z,0,debugSize)] = remap->GenSingle2D(x,z,seed);
        }
    }
    glBindTexture(GL_TEXTURE_2D, debugTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, debugSize,debugSize, 0, GL_RED, GL_UNSIGNED_BYTE, debugValues);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void terrainGenerator::generateNearbyChunks(std::vector<Chunk>& chunks) {
    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    int maxChunkDistance = 2;
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
    if (chunk.y > 0) {
        // just air
        return;
    }
    if (chunk.y < 0) {
        chunk.fillUnits(0,0,0,16,16,16,Unit::STONE);
        chunk.fillUnits(0,2,-8,8,7,0,Unit::ORE);
        return;
    }
    // else
    chunk.fillUnits(0,7,0,16,8,16,Unit::GRASS);
    chunk.fillUnits(0,5,0,16,7,16,Unit::DIRT);
    chunk.fillUnits(0,0,0,16,5,16,Unit::STONE);
    chunk.fillUnits(0,0,0,8,5,8,Unit::ORE);


    // if (chunk.y > 1) return;
    // if (chunk.y < -1) {
    //     chunk.fillUnits(0,0,0,Chunk::LENGTH,Chunk::LENGTH,Chunk::LENGTH,Unit::GRASS);
    //     return;
    // }
    // float perlinValues[Chunk::LENGTH*Chunk::LENGTH];
    // // const float unitPoses[Chunk::LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    // // remap->GenPositionArray2D(perlinValues,Chunk::LENGTH*Chunk::LENGTH,
    // //     unitPoses,unitPoses,
    // //     chunk.x*Chunk::LENGTH,chunk.z*Chunk::LENGTH,seed);
    // remap->GenUniformGrid2D(perlinValues,chunk.x*Chunk::LENGTH,chunk.z*Chunk::LENGTH,
    //     Chunk::LENGTH,Chunk::LENGTH,1,1,seed);
    //
    // for (int x = 0; x < Chunk::LENGTH; ++x) {
    //     for (int z = 0; z < Chunk::LENGTH; ++z) {
    //         int val = perlinValues[posToIdx(x,z,0,Chunk::LENGTH)];
    //         int localV = val-chunk.y*Chunk::LENGTH;
    //         localV = (localV<0?0:(localV>Chunk::LENGTH?Chunk::LENGTH:localV));
    //         Unit unit = Unit::DIRT;//val%3==0?Unit::DIRT:(val%3==1?Unit::ORE:Unit::STONE);
    //         chunk.fillUnits(x,localV-1,z,x+1,localV,z+1,unit);
    //     }
    // }
}
