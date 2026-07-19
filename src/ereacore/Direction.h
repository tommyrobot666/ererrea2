#pragma once

#include <glm/glm.hpp>

namespace Direction {
    constexpr glm::ivec3 UP = glm::vec3(0,1,0);

    constexpr int ALL_SIZE = 6;
    const glm::ivec3* ALL = new glm::ivec3[ALL_SIZE]{UP,UP,UP,UP,UP,UP,};
    const std::vector<glm::ivec3> ALL_VEC = {ALL,ALL+ALL_SIZE};
}