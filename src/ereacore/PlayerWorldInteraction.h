#pragma once

#include <vector>
#include <ereacore/chunk.h>
#include <ereacore/keybinds.h>
#include <glm/vec3.hpp>
#include <3dListUtil.h>

class PlayerWorldInteraction {
    Keybinds keybinds;
    public:
    void moveCamera();
    void interactWithUnits(std::vector<Chunk>& chunks);
    static ListUtilVecInt rayCast(std::vector<Chunk>& chunks, glm::vec3& rayDir, glm::vec3& rayStartPos, int maxChunkDistance);
    static ListUtilVecInt* rayCastNullable(std::vector<Chunk>& chunks, glm::vec3& rayDir, glm::vec3& rayStartPos, int maxChunkDistance);
};
