#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

class vertexObject {
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertices;
    unsigned int triangles; // use for glDrawArrays

    vertexObject(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int vertices, unsigned int triangles);

    ~vertexObject();

    void draw();
};


class renderer {
    std::vector<vertexObject*> vertexObjects = std::vector<vertexObject*>();
    unsigned int shaderProgram;
    unsigned int transformLoc;

public:
    renderer();

    static unsigned int setUpShaders();

    static vertexObject* createVertexObject(float vertices[], unsigned int indices[], unsigned int sizeOfVertices,
                                          unsigned int sizeOfIndices);

    void setShaderTransform(glm::mat4 *trans) const;

    void defaultShader() const;

    static unsigned int loadPngTexture(std::string path);
};
