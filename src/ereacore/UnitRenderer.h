#pragma once

#include <vector>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class UnitRenderer {
    glm::vec3 unitFaces[6] = {};
public:
    class Vertex {
    public:
        Vertex(glm::vec3 pos,glm::vec2 uv) : pos(pos),uv(uv) {};
        glm::vec3 pos;
        glm::vec2 uv;

        bool operator==(const Vertex & value) const {
            return pos == value.pos && uv == value.uv;
        };
    };


    VertexObject* cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    bool loaded = false;
    UnitRenderer() : cubeModel(), grassTexture(), dirtTexture(), stoneTexture(), oreTexture() {};
    ~UnitRenderer();
    void render(std::vector<Chunk>& chunks, glm::mat4& proj);
    void get_or_add_vertex(std::vector<UnitRenderer::Vertex> vertices, UnitRenderer::Vertex v, int &i);
    VertexObject* generateChunkMesh(Chunk& chunk);
    void generateQuadCache();
    void load();
};
