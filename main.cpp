#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

#ifndef RESOURCES_PATH
#define RESOURCES_PATH
#endif

const char* defaultVertexShaderSource = {
    #include <shaders/default.vert>
};
const char* defaultFragmentShaderSource = {
    #include <shaders/default.frag>
};

void simulateFrame(GLFWwindow *window);
void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram, unsigned int texture, unsigned int transformLoc, glm::vec3 cubePositions[]);
unsigned int setUpShaders();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, "tErerrea2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // this starts glad, but it's in a messy if because it also returns if it worked
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //functions can be casted?
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0,0,GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT);

    unsigned int shaderProgram = setUpShaders();
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    float vertices[] = {
        // positions           // colors       // uvs
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, 0.5f,    1.0f,1.0f,1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,1.0f,1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,1.0f,1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 
    //float texCoords[] = {0.0f, 0.0f,  1.0f, 0.0f,  0.5f, 1.0f  }; for triangle
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // tell it how vertices is formatted
    // position attribute
    // layout location, items, type, idk, stride length, offset. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    //GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER (like MonoGame)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  // for GL_CLAMP_TO_BORDER 

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
    int width, height, nrChannels;
    unsigned char *data = stbi_load(RESOURCES_PATH"smile.png", &width, &height, &nrChannels, STBI_rgb_alpha); 
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    if (data){
        // texture target, mipmap levels, load in format, size, idk, stored in format, data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    glEnable(GL_DEPTH_TEST);


    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    // glm::mat4 view;
    // view = glm::lookAt(cameraPos, cameraTarget, up);

    while(!glfwWindowShouldClose(window))
    {
        simulateFrame(window);
        renderFrame(window,VAO,shaderProgram,texture,transformLoc,cubePositions);

        glfwSwapBuffers(window); //it draws to back buffer first, this swaps it to be the shown front buffer
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window);
void simulateFrame(GLFWwindow *window){
    processInput(window);
}

void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram, unsigned int texture, unsigned int transformLoc, glm::vec3 cubePositions[]){
    double time = glfwGetTime();

    glClearColor(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // std::cout << std::sin(time) << "   " << time << "\n";

    // camera location
    // glm::mat4 camera = glm::mat4(1.0f);
    // camera = glm::translate(camera, glm::vec3(0.0f,0.0f,0.4f));
    // circle around camera
    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    // fov, aspect ratio, near plane distance, far plane distance
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    for (size_t i = 0; i < 10; i++){
        // object transform
        glm::mat4 trans = glm::mat4(1.0f); // identity
        trans = glm::translate(trans, cubePositions[i]); // now translation
        trans = glm::rotate(trans, (float)time, glm::vec3(0.0, 0.5, 1.0)); // also rotate
        trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f)); // and scale

        // trans is reused as final position
        trans = proj * view * trans;

        glm::vec4 out = trans * glm::vec4(1.0f);
        std::cout << out[0] << "," << out[1] << "," << out[2] << " :" << i <<"\n";

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
    }
    glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int setUpShaders(){
    int  success;
    char infoLog[512];
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &defaultVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Default vertex shader failed\n" << infoLog;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Default fragment shader failed\n" << infoLog;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program failed\n" << infoLog;
    }
    glDeleteShader(vertexShader); // the shaders are in the program now
    glDeleteShader(fragmentShader);
    return shaderProgram;
}