#pragma once
#include <vector>
#include <core/scene.h>
#include <ereacore/chunk.h>
#include <ereacore/terrainGenerator.h>
#include <ereacore/BlockRenderer.h>

class ereaGameScene : public Scene {
    std::vector<Chunk> chunks;
    BlockRenderer blockRenderer = {};
    terrainGenerator chunkGenerator;
    void interactWithUnits();
    public:
    void load() override;
    void simulate() override;
    void render() override;
    void close() override;
};
