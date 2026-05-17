#include <ereacore/chunk.h>
#include <3dListUtil.h>

Chunk::Chunk(const int x, const int y, const int z) : x(x), y(y), z(z) {}

void Chunk::setUnit(int x, int y, int z,Unit unit) {
    units[posToIdx(x,y,z,LENGTH)] = unit;
}

void Chunk::fillUnits(int x1, int y1, int z1, int x2, int y2, int z2,Unit unit) {
    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            for (int z = z1; z <= z2; ++z) {
                units[posToIdx(x,y,z,LENGTH)] = unit;
            }
        }
    }
}

