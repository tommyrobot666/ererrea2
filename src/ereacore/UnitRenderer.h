#pragma once

#include <vector>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class UnitRenderer {
    glm::vec3 unitFaces0[6] = {};
    glm::vec3 unitFaces1[6] = {};
public:
    VertexObject* cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    bool loaded = false;
    UnitRenderer() : cubeModel(), grassTexture(), dirtTexture(), stoneTexture(), oreTexture() {};
    ~UnitRenderer();
    void render(std::vector<Chunk>& chunks, glm::mat4& proj);
    VertexObject* generateChunkMesh(Chunk& chunk);
    void generateQuadCache();
    void load();

    class Vertex {
        public:
        Vertex(glm::vec3 pos,glm::vec2 uv,glm::vec4 atlas) : pos(pos),uv(uv),atlas(atlas) {};
        glm::vec3 pos;
        glm::vec2 uv;
        glm::vec4 atlas;
    };
};
