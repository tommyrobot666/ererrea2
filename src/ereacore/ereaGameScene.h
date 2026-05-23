#pragma once
#include <vector>
#include <core/scene.h>
#include <core/renderer.h>
#include <ereacore/chunk.h>
// #include <ereacore/terrainGenerator.h>

class ereaGameScene : public Scene {
    std::vector<Chunk> chunks;
    VertexObject* cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    // terrainGenerator chunkGenerator;
    void interactWithUnits();
    public:
    void load() override;
    void simulate() override;
    void render() override;
    void close() override;
};
