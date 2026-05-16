#pragma once
#include <scene.h>
#include <glm/glm.hpp>

#include <renderer.h>

class otherTestScene : public scene{
    unsigned int texture;
    vertexObject* VO;
    unsigned int shaderProgram;
    unsigned int transformLoc;
    // remember not to put const on a static var
    static glm::vec3 * cubePoses();

public:
    void load() override;

    void simulate() override;

    void render() override;
};
