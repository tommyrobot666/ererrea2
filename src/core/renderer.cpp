#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <stb_image.h>

#include <core/renderer.h>
#include <core/vertexObjectGenerators.h>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH
#endif


VertexObject::VertexObject(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int vertices,
                           unsigned int triangles) : VAO(VAO), VBO(VBO), EBO(EBO), vertices(vertices), triangles(triangles) {}

VertexObject::VertexObject() : VAO(-1), VBO(-1), EBO(-1), vertices(0), triangles(0) {}

VertexObject::~VertexObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void VertexObject::currentBind() {
    glBindVertexArray(VAO);
}

void VertexObject::draw() {
    glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);
}

void VertexObject::drawArray() {
    glDrawArrays(GL_TRIANGLES, 0, triangles);
}

void Vertex::getOrAddVertex(std::vector<Vertex> &vertices, Vertex &v, unsigned int &i) {
    bool alreadyExists = false;
    for (int j = 0; j < vertices.size(); ++j) {
        auto vertex = vertices[j];
        if (v == vertex) {
            alreadyExists = true;
            i = j;
        }
    }
    if (!alreadyExists) {
        i = vertices.size();
        vertices.push_back(v);
    }
}

void Vertex::convertToFloats(std::vector<Vertex> &vertices, std::vector<float> &verticesFloats) {
    for (Vertex & vertex : vertices) {
        verticesFloats.push_back(vertex.pos.x);
        verticesFloats.push_back(vertex.pos.y);
        verticesFloats.push_back(vertex.pos.z);
        verticesFloats.push_back(1.f);
        verticesFloats.push_back(1.f);
        verticesFloats.push_back(1.f);
        verticesFloats.push_back(vertex.uv.x);
        verticesFloats.push_back(vertex.uv.y);
    }
}


unsigned int Renderer::setUpShaders() {
    int success;
    char infoLog[512];
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream vertFile(RESOURCES_PATH "shaders/default.vert");
    std::stringstream vertFileStream;
    vertFileStream << vertFile.rdbuf();
    vertFile.close();
    std::string vertFileData = vertFileStream.str();
    const char* vertFileDataChar = vertFileData.c_str();
    glShaderSource(vertexShader, 1, &vertFileDataChar, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Default vertex shader failed\n" << infoLog;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream fragFile(RESOURCES_PATH "shaders/default.frag");
    std::stringstream fragFileStream;
    fragFileStream << fragFile.rdbuf();
    fragFile.close();
    std::string fragFileData = fragFileStream.str();
    const char* fragFileDataChar = fragFileData.c_str();
    glShaderSource(fragmentShader, 1, &fragFileDataChar, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Default fragment shader failed\n" << infoLog;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program failed\n" << infoLog;
    }
    glDeleteShader(vertexShader); // the shaders are in the program now
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void setUpTextureDrawing() {
    //GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER (like MonoGame)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // for GL_CLAMP_TO_BORDER

    //mipmap stuff
    // NEAREST is exact value, LINEAR blurrs between
    // GL_[MIPMAP]_MIPMAP_[TEXEL]
    // [MIPMAP] affects interpoliate for mipmap selection
    // [TEXEL] affects interpoliate for pixels
    // 2*2 = 4 mipmap constants for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_x_FILTER, x)

    // GL_NEAREST is exact value, GL_LINEAR blurrs between
    // GL_TEXTURE_MIN_FILTER is scaled down, GL_TEXTURE_MAG_FILTER is scaled up
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::textureDrawingNearest() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Renderer::Renderer() {
    shaderProgram = setUpShaders();
    transformLoc = glGetUniformLocation(shaderProgram, "transform");

    setUpTextureDrawing();

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
}

VertexObject* Renderer::createVertexObject(float vertices[], unsigned int indices[], unsigned int sizeOfVertices,
                                          unsigned int sizeOfIndices) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

    // tell it how vertices is formatted
    // position attribute
    // layout location, items, type, idk, stride length, offset.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return new VertexObject{VAO, VBO, EBO, static_cast<unsigned int>(sizeOfIndices/sizeof(int)), sizeOfVertices/vertexObjectGenerators::SizeOfVertex};
}

void Renderer::setShaderTransform(glm::mat4 *trans) const {
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(*trans));
}

void Renderer::defaultShader() const {
    glUseProgram(shaderProgram);
}

unsigned int Renderer::loadPngTexture(std::string path) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load((RESOURCES_PATH "textures/" + path).c_str(), &width, &height, &nrChannels,
                                    STBI_rgb_alpha);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    if (data) {
        // texture target, mipmap levels, load in format, size, idk, stored in format, data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture\n" << stbi_failure_reason();
    }
    stbi_image_free(data);
    return texture;
}

unsigned char *Renderer::loadPngData(std::string path) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load((RESOURCES_PATH "textures/" + path).c_str(), &width, &height, &nrChannels,
                                    STBI_rgb_alpha);
    return data;
}

unsigned int Renderer::loadPngTextureNearest(std::string path) {
    unsigned int ret = loadPngTexture(path);
    textureDrawingNearest();
    return ret;
}

unsigned int Renderer::createTextureAtlas(unsigned char **allData, int textures, int length) {
    // 4 == numChannels
    int atlasLengthTextures = std::ceil(std::sqrt(textures));
    int atlasLengthPixels = atlasLengthTextures * length;
    int atlasDataSize = atlasLengthPixels*atlasLengthPixels*4;
    std::vector<unsigned char> atlasData;
    atlasData.reserve(atlasDataSize);
    for (int x = 0; x < atlasLengthPixels; x++) {
        for (int y = 0; y < atlasLengthPixels; y++) {
            int idx = x + y*atlasLengthPixels;
            int texturesIdx = x/length + y/length*atlasLengthTextures;
            int textureX = x%length;
            int textureY = y%length;
            int textureIdx = textureX + textureY*length;

            atlasData[idx*4] = allData[texturesIdx][textureIdx*4];
            atlasData[idx*4+1] = allData[texturesIdx][textureIdx*4+1];
            atlasData[idx*4+2] = allData[texturesIdx][textureIdx*4+2];
            atlasData[idx*4+3] = allData[texturesIdx][textureIdx*4+3];
        }
    }

    unsigned int atlasTexture;
    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glActiveTexture(GL_TEXTURE0);
    // texture target, mipmap levels, load in format, size, idk, stored in format, data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasLengthPixels, atlasLengthPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    return atlasTexture;
}

void Renderer::currentTexture(unsigned int texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::freeTexture(unsigned int texture) {
    glDeleteTextures(1, &texture);
}
