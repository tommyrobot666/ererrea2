#pragma once

inline int posToIdx(int x, int y, int z, int size) {
    return x + y*size + z*size*size;
}

struct ListUtilVec {
    int x,y,z;
};

inline ListUtilVec stepGridPos(int x, int y, int z, int size) {
    return ListUtilVec{x/size,y/size,z/size};
}