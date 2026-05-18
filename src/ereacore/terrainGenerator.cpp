#include <random>
#include <3dListUtil.h>
#include <ereacore/terrainGenerator.h>

double terrainGenerator::sample2dNoise(int x, int y) {
    long seed = x+(static_cast<long>(y)<<32);
    rnd.seed(seed);
    return uni(rnd);
}

double terrainGenerator::linear2dNoise(double x, double y) {
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
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double dista = dist(ListUtilVec{x-xStart+i,y-yStart+j,0});
            int idx = posToIdx(i,j,0,3);
            // if (dista == 0) return values[idx];

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
    for (int x = 0; x < Chunk::LENGTH; ++x) {
        for (int z = 0; z < Chunk::LENGTH; ++z) {
            double scaled = 2.3;
            double val = linear2dNoise((static_cast<double>(x)/Chunk::LENGTH + chunk.x)*scaled, (static_cast<double>(z)/Chunk::LENGTH + chunk.z)*scaled);
            int globalV = (val*Chunk::LENGTH*4)-Chunk::LENGTH*3;
            int localV = globalV-chunk.y*Chunk::LENGTH;
            if (localV < 0) localV = 0; if (localV > Chunk::LENGTH) localV = Chunk::LENGTH;
            Unit unit = globalV%3==0?Unit::DIRT:(globalV%3==1?Unit::ORE:Unit::STONE);
            chunk.fillUnits(x,0,z,x+1,localV,z+1,unit);
        }
    }
}
