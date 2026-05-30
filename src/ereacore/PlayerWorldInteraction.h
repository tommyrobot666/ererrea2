#pragma once

#include <vector>
#include <ereacore/chunk.h>

class PlayerWorldInteraction {
    public:
    void moveCamera();
    void interactWithUnits(std::vector<Chunk>& chunks);
};
