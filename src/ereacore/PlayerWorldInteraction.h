#pragma once

#include <vector>
#include <ereacore/chunk.h>
#include <ereacore/keybinds.h>
#include <glm/vec3.hpp>

class PlayerWorldInteraction {
    Keybinds keybinds;
    int lastChangex,lastChangey,lastChangez;
    public:
    glm::ivec3 lastLookedBlock;
    PlayerWorldInteraction() : lastChangex(), lastChangey(), lastChangez(), lastLookedBlock() {};
    void moveCamera();
    void interactWithUnits(std::vector<Chunk>& chunks);
    static glm::ivec3 rayCast(std::vector<Chunk>& chunks, glm::vec3& rayDir, glm::vec3& rayStartPos, int maxChunkDistance);
    static glm::ivec3* rayCastNullable(std::vector<Chunk>& chunks, glm::vec3& rayDir, glm::vec3& rayStartPos, int maxChunkDistance);
};
