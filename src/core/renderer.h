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
    ~VertexObject();
    void currentBind() const;
    void draw() const;
    void drawArray() const;
    void drawAt(glm::mat4& t) const;
    void bindAndDraw() const;
    void bindAndDrawAt(glm::mat4& t) const;
};

class Vertex {
public:
    Vertex(glm::vec3 pos,glm::vec2 uv) : pos(pos),uv(uv) {};
    glm::vec3 pos;
    glm::vec2 uv;

    bool operator==(const Vertex & value) const {
        return pos == value.pos && uv == value.uv;
    }

    static void getOrAddVertex(std::vector<Vertex> &vertices, const Vertex &v, unsigned int &i);
    static void convertToFloats(std::vector<Vertex> &vertices, std::vector<float> &verticesFloats);;
};

class VertexList {
    public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // returns the vertexe's index
    unsigned int getOrAddVertex(Vertex &v)
    {unsigned int i = 0; Vertex::getOrAddVertex(vertices, v, i); return i;}
    void addTriangle(Vertex &v0, Vertex &v1, Vertex &v2); // use getOrAddVertex for this
    void addTriangleNoRef(Vertex v0, Vertex v1, Vertex v2);
    std::vector<float> convertVerticesToFloats()
    {std::vector<float> verticesFloats; Vertex::convertToFloats(vertices, verticesFloats); return verticesFloats;}
    void scaleAndTransformUV(float deltaU, float deltaV, float scaleU, float scaleV);
    VertexObject* makeObject();
};

class Renderer {
    static void setUpTextureDrawing();
public:
    unsigned int shaderProgram;
    unsigned int transformLoc;
    unsigned int d2texture0;
    unsigned int boundVAO;

    Renderer(): shaderProgram(), transformLoc(), d2texture0(), boundVAO() {};
    void init();
    static unsigned int setUpShaders();
    static VertexObject* createVertexObject(float vertices[], unsigned int indices[], unsigned int sizeOfVertices,
                                            unsigned int sizeOfIndices);
    static void setShaderTransform(glm::mat4 *trans);
    static void defaultShader();
    static unsigned int loadPngTexture(const std::string& path);
    static unsigned char *loadPngData(const std::string& path);
    static unsigned int loadPngTextureNearest(const std::string& path); // include file extension in path
    static void currentTexture(unsigned int texture);
    static unsigned int getCurrentTexture();
    static void clear(float r, float g, float b, float a);
    static void freeTexture(unsigned int texture);
    static void textureDrawingNearest();
    static unsigned int createTextureAtlas(unsigned char **data, int textures, int length);
    static void disableDepthTest();
    static void enableDepthTest();
};

extern Renderer renderer;