#pragma once

#include <vector>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class UnitRenderer {
    static glm::vec3 unitFaces[6*4];
public:
    struct meshCacheEntry {
        glm::ivec3 pos;
        VertexObject* mesh;
    };

    std::vector<meshCacheEntry> chunkMeshCache;
    VertexObject* cubeModel;
    unsigned int atlasTexture;
    bool loaded = false;
    UnitRenderer() : cubeModel(), atlasTexture() {};
    ~UnitRenderer();
    void render(std::vector<Chunk>& chunks, glm::mat4& proj);
    VertexObject* generateChunkMesh(Chunk& chunk);
    void load();
};
