#pragma once
#include <gameState.h>

class scene
{
protected:
    gameState* gs;
public:
    scene(gameState* gs);

    virtual ~scene();

    virtual void load();

    virtual void simulate();

    virtual void render();
};
