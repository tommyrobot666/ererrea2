#pragma once
#include <gameState.h>
#include <scene.h>

class scene
{
public:
    scene();

    virtual ~scene();

    virtual void load();

    virtual void simulate();

    virtual void render();
};
