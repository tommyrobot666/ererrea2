#pragma once

#include <vector>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class BlockRenderer {
public:
    VertexObject* cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    void render(std::vector<Chunk>& chunks, glm::mat4& proj);
    void load();
    void close();
};
