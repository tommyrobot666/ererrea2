#pragma once

#include <glm/glm.hpp>

#include <string>

class VertexObject {
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertices;
    unsigned int triangles; // use for glDrawArrays

    VertexObject(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int vertices, unsigned int triangles);

    VertexObject();

    ~VertexObject();

    void currentBind();

    void draw();

    void drawArray();
};


class Renderer {
public:
    unsigned int shaderProgram;
    unsigned int transformLoc;

    Renderer();

    static unsigned int setUpShaders();

    static VertexObject createVertexObject(float vertices[], unsigned int indices[], unsigned int sizeOfVertices,
                                          unsigned int sizeOfIndices);

    void setShaderTransform(glm::mat4 *trans) const;

    void defaultShader() const;

    static unsigned int loadPngTexture(std::string path);

    static unsigned int loadPngTextureNearest(std::string path);

    static void currentTexture(unsigned int texture);

    static void clear(float r, float g, float b, float a);

    static void freeTexture(unsigned int texture);

    static void textureDrawingNearest();
};
