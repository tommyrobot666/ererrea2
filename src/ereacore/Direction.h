#pragma once

#include <glm/glm.hpp>

namespace Direction {
    constexpr glm::vec3 UP = glm::vec3(0,1,0);

    constexpr int ALL_SIZE = 6;
    const glm::vec3* ALL = new glm::vec3[ALL_SIZE]{UP,UP,UP,UP,UP,UP,};
    const std::vector<glm::vec3> ALL_VEC = {ALL,ALL+ALL_SIZE};
}