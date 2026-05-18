#pragma once
#include <cmath>

inline int posToIdx(int x, int y, int z, int size) {
    return x + y*size + z*size*size;
}

struct ListUtilVecInt {
    int x,y,z;
};

struct ListUtilVec {
    double x,y,z;
};

inline ListUtilVecInt stepGridPos(int x, int y, int z, int size) {
    return ListUtilVecInt{x/size,y/size,z/size};
}

inline double dist(ListUtilVec vec) {
    return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}