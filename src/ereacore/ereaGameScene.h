#pragma once
#include <vector>
#include <core/scene.h>
#include <core/renderer.h>
#include <ereacore/chunk.h>

class ereaGameScene : public Scene {
    std::vector<Chunk> chunks;
    VertexObject* cubeModel;
    public:
    void load() override;
    void simulate() override;
    void render() override;
    void close() override;
};
