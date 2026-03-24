#include <gameState.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

class scene
{
protected:
    gameState* gs;
public:
    scene(gameState* gs){
        this->gs = gs;
    }
    virtual ~scene() = default;

    virtual void simulate(){
        // The code that I wrote when folowing the tutorial
        GLFWwindow* window = (*gs).window;
        glm::vec3 cameraPos = (*gs).cameraPos;
        glm::vec3 cameraDir = (*gs).cameraDir;
        float yaw = (*gs).yaw;
        float pitch = (*gs).pitch;
        float mouseVelX = (*gs).mouseVelX;
        float mouseVelY = (*gs).mouseVelY;

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        const float cameraSpeed = 0.05f; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            yaw -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            yaw += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            pitch -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            pitch += 0.1f;
        
        yaw -= mouseVelX*0.005;
        pitch -= mouseVelY*0.005;

        (cameraDir).x = cos(yaw) * cos(pitch);
        (cameraDir).y = sin(pitch);
        (cameraDir).z = sin(yaw) * cos(pitch);
    }

    virtual void render(){
        // The code that I wrote when folowing the tutorial
        GLFWwindow* window = (*gs).window;
        glm::vec3 cameraPos = (*gs).cameraPos;
        glm::vec3 cameraDir = (*gs).cameraDir;
        unsigned int texture; // use the new render class
        unsigned int VAO; // use the new render class

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

        double time = glfwGetTime();

        glClearColor(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera location
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos+cameraDir, glm::vec3(0.0, 1.0, 0.0));

        // fov, aspect ratio, near plane distance, far plane distance
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

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

            // set shader transform

            glDrawArrays(GL_TRIANGLES, 0, 36);
            
        }
        glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again
    }
};
