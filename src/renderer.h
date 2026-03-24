#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

class vertexObject
{
    public:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int vertices;
        unsigned int triangles; // use for glDrawArrays

        vertexObject(unsigned int VAO,unsigned int VBO,unsigned int EBO,unsigned int vertices,unsigned int triangles);

        ~vertexObject();

        void draw();

};


class renderer
{
private:
    std::vector<vertexObject> vertexObjects;
    unsigned int shaderProgram;
    unsigned int transformLoc;

    unsigned int setUpShaders();
public:
    renderer();
    vertexObject createVertexObject(double vertices[], unsigned int indices[], unsigned int vertCount, unsigned int triangles);
    void setShaderTransform(glm::mat4* trans) const;
    void defaultShader() const;
    static unsigned int loadPngTexture(std::string path);
};