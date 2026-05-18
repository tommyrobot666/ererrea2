#pragma once
#include <vector>
#include <core/scene.h>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class ereaGameScene : public Scene {
    std::vector<Chunk> chunks;
    VertexObject* cubeModel;
    unsigned int grassTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;
    unsigned int oreTexture;
    void interactWithUnits();
    public:
    void load() override;
    void simulate() override;
    void render() override;
    void close() override;
};
