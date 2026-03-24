#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const char* defaultVertexShaderSource = {
    #include <shaders/default.vert>
};
const char* defaultFragmentShaderSource = {
    #include <shaders/default.frag>
};

class vertexObject
{
    public:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int vertices;
        unsigned int triangles; // use for glDrawArrays

        vertexObject(unsigned int VAO,unsigned int VBO,unsigned int EBO){
            this->VAO = VAO;
            this->VBO = VBO;
            this->EBO = EBO;
        }

        ~vertexObject(){
            glDeleteVertexArrays(1,&VAO);
            glDeleteBuffers(1,&VBO);
            glDeleteBuffers(1,&EBO);
        }

        void draw(){
            glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);
        }

};


class renderer
{
private:
    std::vector<vertexObject> vertexObjects;
    unsigned int shaderProgram;
    unsigned int transformLoc;

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
public:
    renderer(){
        shaderProgram = setUpShaders();
        transformLoc = glGetUniformLocation(shaderProgram, "transform");

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
    }

    vertexObject createVertexObject(double vertices[], unsigned int indices[]){
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
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

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //GL_STATIC_DRAW is for unchanging things, GL_DYNAMIC_DRAW is for changing things
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

        return vertexObject(VAO,VBO,EBO);
    }

    void setShaderTransform(&mat4 trans){
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    }

    void defaultShader(){
        glUseProgram(shaderProgram);
    }
};