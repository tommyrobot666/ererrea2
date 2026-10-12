#include <ereacore/chunk.h>
#include <3dListUtil.h>


void Chunk::globalPosHandleNegatives(glm::ivec3 &chunkPos, int &localPosX, int &localPosY, int &localPosZ) {
    if (localPosX < 0) {
        chunkPos.x--;
        localPosX = LENGTH+localPosX+1;
    }
    if (localPosY < 0) {
        chunkPos.y--;
        localPosY = LENGTH+localPosY+1;
    }
    if (localPosZ < 0) {
        chunkPos.z--;
        localPosZ = LENGTH+localPosZ+1;
    }
    // std::cout << chunkPos.x << "," << chunkPos.y << "," << chunkPos.z << ":" << localPosX << "," << localPosY << "," << localPosZ << "\n";
}

Chunk::Chunk(const int x, const int y, const int z) : x(x), y(y), z(z), chunkPos(x, y, z) {}

void Chunk::setUnit(const int lx, const int ly, const int lz, const Unit unit) {
    if (unit != getUnit(lx,ly,lz)) {
        units[posToIdx(lx,ly,lz,LENGTH)] = unit;
        queueMeshRegen = true;
    }
}

void Chunk::fillUnits(const int lx1, const int ly1, const int lz1, const int lx2, const int ly2, const int lz2, const Unit unit) {
    for (int x = lx1; x < lx2; ++x) {
        for (int y = ly1; y < ly2; ++y) {
            for (int z = lz1; z < lz2; ++z) {
                units[posToIdx(x,y,z,LENGTH)] = unit;
            }
        }
    }
}

Unit Chunk::getUnit(const int lx, const int ly, const int lz) const {
    return units[posToIdx(lx,ly,lz,LENGTH)];
}

bool Chunk::inBounds(const int lx, const int ly, const int lz) {
    return lx>=0&&lx<LENGTH&&ly>=0&&ly<LENGTH&&lz>=0&&lz<LENGTH;
}

Chunk* Chunk::findChunkOrNone(std::vector<Chunk>& chunks, int cx, int cy, int cz) {
    for (auto & chunk : chunks) {
        if (chunk.x == cx && chunk.y == cy && chunk.z == cz) {
            return &chunk;
        }
    }
    return nullptr;
}

/// Returns air if chunk doesn't exist
Unit Chunk::getUnitAtGlobalPos(std::vector<Chunk> &chunks, int gx, int gy, int gz) {
    auto chunkPos = stepGridPos(gx,gy,gz,LENGTH);
    int localPosX = gx-chunkPos.x*LENGTH, localPosY = gy-chunkPos.y*LENGTH, localPosZ = gz-chunkPos.z*LENGTH;
    globalPosHandleNegatives(chunkPos,localPosX,localPosY,localPosZ);
    // std::cout << chunkPos.x << "," << chunkPos.y << "," << chunkPos.z << ":" << localPosX << "," << localPosY << "," << localPosZ << "\n";
    auto chunk = findChunkOrNone(chunks,chunkPos.x, chunkPos.y, chunkPos.z);
    if (chunk) {
        return chunk->getUnit(localPosX, localPosY, localPosZ);
    }
    return Unit::NONE;
}

/// Does nothing if chunk doesn't exist
void Chunk::setUnitAtGlobalPos(Unit unit, std::vector<Chunk> &chunks, int gx, int gy, int gz) {
    auto chunkPos = stepGridPos(gx,gy,gz,LENGTH);
    int localPosX = gx-chunkPos.x*LENGTH, localPosY = gy-chunkPos.y*LENGTH, localPosZ = gz-chunkPos.z*LENGTH;
    globalPosHandleNegatives(chunkPos,localPosX,localPosY,localPosZ);
    auto chunk = findChunkOrNone(chunks,chunkPos.x, chunkPos.y, chunkPos.z);
    if (chunk) {
        chunk->setUnit(localPosX, localPosY, localPosZ,unit);
    }
}

