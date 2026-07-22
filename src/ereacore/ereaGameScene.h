#pragma once
#include <vector>
#include <core/scene.h>
#include <uicore/uiObject.h>
#include <ereacore/chunk.h>
#include <ereacore/terrainGenerator.h>
#include <ereacore/UnitRenderer.h>
#include <ereacore/PlayerWorldInteraction.h>

class ereaGameScene : public Scene {
    std::vector<Chunk> chunks;
    UnitRenderer unitRenderer;
    terrainGenerator chunkGenerator;
    PlayerWorldInteraction playerWorldInteraction;
    uiObject testUi = uiObject();
    public:
    ~ereaGameScene() override = default;
    void load() override;
    void simulate() override;
    void render() override;
    void close() override;
};
