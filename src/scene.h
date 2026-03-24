#pragma once

class scene
{
public:
    scene();

    virtual ~scene();

    virtual void load();

    virtual void simulate();

    virtual void render();
};
