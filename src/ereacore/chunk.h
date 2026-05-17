#pragma once

#include <ereacore/unit.h>

class Chunk {
    public:
    static constexpr int LENGTH = 16;
    const int x,y,z;
    Unit units[LENGTH*LENGTH*LENGTH] = {};

    Chunk(int x,int y,int z);

    void setUnit(int x,int y,int z,Unit unit);

    void fillUnits(int x1,int y1,int z1,int x2,int y2,int z2,Unit unit);
};