#pragma once

#include <glm/glm.hpp>

namespace Direction {
    constexpr glm::ivec3 UP = glm::vec3(0,1,0);
    constexpr glm::ivec3 DOWN = glm::vec3(0,-1,0);
    constexpr glm::ivec3 WEST = glm::vec3(1,0,0);
    constexpr glm::ivec3 EAST = glm::vec3(-1,0,0);
    constexpr glm::ivec3 NORTH = glm::vec3(0,0,1);
    constexpr glm::ivec3 SOUTH = glm::vec3(0,0,-1);

    constexpr int ALL_SIZE = 6;
    inline const glm::ivec3* ALL = new glm::ivec3[ALL_SIZE]{UP,DOWN,WEST,EAST,NORTH,SOUTH,};
    const std::vector<glm::ivec3> ALL_VEC = {ALL,ALL+ALL_SIZE};
}