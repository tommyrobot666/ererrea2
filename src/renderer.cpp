#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#include <stb_image.h>

#include <renderer.h>

#ifndef RESOURCES_PATH
#define RESOURCES_PATH
#endif

const char *defaultVertexShaderSource = {
#include <shaders/default.vert>
};
const char *defaultFragmentShaderSource = {
#include <shaders/default.frag>
};


vertexObject::vertexObject(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int vertices,
                           unsigned int triangles) {
    this->VAO = VAO;
    this->VBO = VBO;
    this->EBO = EBO;
    this->vertices = vertices;
    this->triangles = triangles;
}

vertexObject::~vertexObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void vertexObject::draw() {
    glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);
}


unsigned int renderer::setUpShaders() {
    int success;
    char infoLog[512];
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &defaultVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Default vertex shader failed\n" << infoLog;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
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

renderer::renderer() {
    shaderProgram = setUpShaders();
    transformLoc = glGetUniformLocation(shaderProgram, "transform");

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

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
}

vertexObject renderer::createVertexObject(float vertices[], unsigned int indices[], unsigned int vertCount,
                                          unsigned int triangles) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
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

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    vertexObject vo = vertexObject(VAO, VBO, EBO, vertCount, triangles);

    vertexObjects.push_back(vo);

    return vo;
}

void renderer::setShaderTransform(glm::mat4 *trans) const {
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(*trans));
}

void renderer::defaultShader() const {
    glUseProgram(shaderProgram);
}

unsigned int renderer::loadPngTexture(std::string path) {
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
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);
    return texture;
}
