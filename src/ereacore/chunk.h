#pragma once

#include <vector>
#include <ereacore/unit.h>
#include <3dListUtil.h>

class Chunk {
    static void globalPosHandleNegatives(ListUtilVecInt &chunkPos, int &localPosX, int &localPosY, int &localPosZ);

    public:
    static constexpr int LENGTH = 16;
    const int x,y,z;
    Unit units[LENGTH*LENGTH*LENGTH] = {};

    Chunk(int x,int y,int z);

    void setUnit(int x,int y,int z,Unit unit);

    void fillUnits(int x1,int y1,int z1,int x2,int y2,int z2,Unit unit);

    Unit getUnit(int x,int y,int z);

    static bool inBounds(int x, int y, int z);

    static Chunk* findChunkOrNone(std::vector<Chunk>& chunks, int x, int y, int z);

    static Unit getUnitAtGlobalPos(std::vector<Chunk>& chunks, int x, int y, int z);

    static void setUnitAtGlobalPos(Unit unit, std::vector<Chunk>& chunks, int x, int y, int z);
};
