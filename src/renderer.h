#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

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
    vertexObject createVertexObject(double vertices[], unsigned int indices[], unsigned int vertices, unsigned int triangles);
    void setShaderTransform(&mat4 trans);
    void defaultShader();
    unsigned int loadPngTexture(String path);
};