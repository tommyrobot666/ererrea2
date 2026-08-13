#pragma once
#include <cmath>
#include <glm/vec3.hpp>

inline int posToIdx(int x, int y, int z, int size) {
    return x + y*size + z*size*size;
}

inline glm::ivec3 stepGridPos(int x, int y, int z, int size) {
    return glm::ivec3{x/size,y/size,z/size};
}

inline double dist(glm::vec3 vec) {
    return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}