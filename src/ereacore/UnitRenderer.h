#pragma once

#include <vector>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class UnitRenderer {
public:
    VertexObject cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    bool loaded = false;
    UnitRenderer() : grassTexture(), dirtTexture(), stoneTexture(), oreTexture() {};
    ~UnitRenderer();
    void render(std::vector<Chunk>& chunks, glm::mat4& proj);
    void load();
};
