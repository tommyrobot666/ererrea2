#pragma once

#include <glm/glm.hpp>

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

    void currentBind();

    void draw();

    void drawArray();
};


class renderer {
public:
    unsigned int shaderProgram;
    unsigned int transformLoc;

    renderer();

    static unsigned int setUpShaders();

    static vertexObject* createVertexObject(float vertices[], unsigned int indices[], unsigned int sizeOfVertices,
                                          unsigned int sizeOfIndices);

    void setShaderTransform(glm::mat4 *trans) const;

    void defaultShader() const;

    static unsigned int loadPngTexture(std::string path);

    static void currentTexture(unsigned int texture);

    static void clear(float r, float g, float b, float a);
};
