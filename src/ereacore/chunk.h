#pragma once

#include <vector>
#include <ereacore/unit.h>
#include <3dListUtil.h>

class Chunk {
    static void globalPosHandleNegatives(glm::ivec3 &chunkPos, int &localPosX, int &localPosY, int &localPosZ);

    public:
    static constexpr int LENGTH = 16;

    const int x,y,z;
    const glm::ivec3 chunkPos;
    Unit units[LENGTH*LENGTH*LENGTH] = {};
    bool queueMeshRegen = false;

    Chunk(int x,int y,int z);
    void setUnit(int lx,int ly,int lz,Unit unit);
    void fillUnits(int lx1,int ly1,int lz1,int lx2,int ly2,int lz2,Unit unit);
    Unit getUnit(int lx,int ly,int lz) const; // NOLINT(*-use-nodiscard)
    static bool inBounds(int lx, int ly, int lz);
    static Chunk* findChunkOrNone(std::vector<Chunk>& chunks, int cx, int cy, int cz);
    static Unit getUnitAtGlobalPos(std::vector<Chunk>& chunks, int gx, int gy, int gz);
    static void setUnitAtGlobalPos(Unit unit, std::vector<Chunk>& chunks, int gx, int gy, int gz);
};
