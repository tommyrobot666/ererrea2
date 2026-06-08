#include <ereacore/chunk.h>
#include <3dListUtil.h>

Chunk::Chunk(const int x, const int y, const int z) : x(x), y(y), z(z) {}

void Chunk::setUnit(int x, int y, int z,Unit unit) {
    units[posToIdx(x,y,z,LENGTH)] = unit;
}

void Chunk::fillUnits(int x1, int y1, int z1, int x2, int y2, int z2,Unit unit) {
    for (int x = x1; x < x2; ++x) {
        for (int y = y1; y < y2; ++y) {
            for (int z = z1; z < z2; ++z) {
                units[posToIdx(x,y,z,LENGTH)] = unit;
            }
        }
    }
}

Unit Chunk::getUnit(int x, int y, int z) {
    return units[posToIdx(x,y,z,LENGTH)];
}

Chunk* Chunk::findChunkOrNone(std::vector<Chunk>& chunks, int x, int y, int z) {
    for (auto & chunk : chunks) {
        if (chunk.x == x && chunk.y == y && chunk.z == z) {
            return &chunk;
        }
    }
    return nullptr;
}

/// Returns air if chunk doesn't exist
Unit Chunk::getUnitAtGlobalPos(std::vector<Chunk> &chunks, int x, int y, int z) {
    auto chunkPos = stepGridPos(x,y,z,LENGTH);
    int localPosX = x-chunkPos.x*LENGTH, localPosY = y-chunkPos.y*LENGTH, localPosZ = z-chunkPos.z*LENGTH;
    auto chunk = findChunkOrNone(chunks,chunkPos.x, chunkPos.y, chunkPos.z);
    if (chunk) {
        return chunk->getUnit(localPosX, localPosY, localPosZ);
    }
    return Unit::NONE;
}

/// Does nothing if chunk doesn't exist
void Chunk::setUnitAtGlobalPos(Unit unit, std::vector<Chunk> &chunks, int x, int y, int z) {
    auto chunkPos = stepGridPos(x,y,z,LENGTH);
    int localPosX = x-chunkPos.x*LENGTH, localPosY = y-chunkPos.y*LENGTH, localPosZ = z-chunkPos.z*LENGTH;
    auto chunk = findChunkOrNone(chunks,chunkPos.x, chunkPos.y, chunkPos.z);
    if (chunk) {
        chunk->setUnit(localPosX, localPosY, localPosZ,unit);
    }
}

