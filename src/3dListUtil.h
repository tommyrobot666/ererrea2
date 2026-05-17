#pragma once

inline int posToIdx(int x, int y, int z, int size) {
    return x + y*size + z*size*size;
}