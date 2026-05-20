#include <FastNoise/FastNoise.h>
#include <random>
#include <3dListUtil.h>
#include <ereacore/terrainGenerator.h>

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

void terrainGenerator::generateChunk(Chunk &chunk) {
    if (chunk.y > 1) return;
    if (chunk.y < -1) {
        chunk.fillUnits(0,0,0,Chunk::LENGTH,Chunk::LENGTH,Chunk::LENGTH,Unit::GRASS);
        return;
    }
    float perlinValues[Chunk::LENGTH*Chunk::LENGTH];
    const float unitPoses[Chunk::LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    // float scaled = 1/0.09;
    // perlin->SetScale(scaled);
    perlin->GenPositionArray2D(perlinValues,Chunk::LENGTH*Chunk::LENGTH,
        unitPoses,unitPoses,
        chunk.x*Chunk::LENGTH,chunk.y*Chunk::LENGTH,seed);

    for (int x = 0; x < Chunk::LENGTH; ++x) {
        for (int z = 0; z < Chunk::LENGTH; ++z) {
            float val = perlinValues[posToIdx(x,z,0,Chunk::LENGTH)];
            int globalV = (val*Chunk::LENGTH*2)-Chunk::LENGTH*1;
            int localV = globalV-chunk.y*Chunk::LENGTH;
            localV = (localV<0?0:(localV>Chunk::LENGTH?Chunk::LENGTH:localV));
            Unit unit = globalV%3==0?Unit::DIRT:(globalV%3==1?Unit::ORE:Unit::STONE);
            chunk.fillUnits(x,0,z,x+1,localV,z+1,unit);
        }
    }
}
