#pragma once
#include <vector>
#include <thread>
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
    std::thread chunkGenerationThread;
    bool chunkGenerationThreadDone = false;
    public:
    ~ereaGameScene() override = default;
    void load() override;
    void simulate() override;
    void makeNewChunkGenerationThread();
    void render() override;
    void drawCubeAtLookedAtUnit(glm::mat4& proj);
    void close() override;
};
