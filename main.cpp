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


float lastMouseX = GAME_WINDOW_WIDTH/2, lastMouseY = GAME_WINDOW_HEIGHT/2;
float mouseVelX = 0, mouseVelY = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    mouseVelX = lastMouseX - xpos;
    mouseVelY = lastMouseY - ypos;

    lastMouseX = xpos;
    lastMouseY = ypos;
}
void simulateFrame(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch, glm::vec3 *cameraDir);
void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram, unsigned int texture, unsigned int transformLoc, glm::vec3 cubePositions[], glm::vec3 *cameraPos, glm::vec3 *cameraDir);

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



    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
    glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.0f, 0.0f, 1.0f));
    float yaw = 0.0f;
    float pitch = 0.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); 

    float lastFrameTime = glfwGetTime();
    float deltaTime = 0;
    while(!glfwWindowShouldClose(window))
    {
        deltaTime = glfwGetTime() - lastFrameTime;

        simulateFrame(window,&cameraPos,&yaw,&pitch,&cameraDir);
        renderFrame(window,VAO,shaderProgram,texture,transformLoc,cubePositions,&cameraPos,&cameraDir);
        mouseVelX = 0;
        mouseVelY = 0;

        glfwSwapBuffers(window); //it draws to back buffer first, this swaps it to be the shown front buffer
        glfwPollEvents();    

        lastFrameTime = glfwGetTime();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch);
void simulateFrame(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch, glm::vec3 *cameraDir){
    (*cameraDir).x = cos(*yaw) * cos(*pitch);
    (*cameraDir).y = sin(*pitch);
    (*cameraDir).z = sin(*yaw) * cos(*pitch);

    processInput(window,cameraPos,yaw,pitch);
}

void renderFrame(GLFWwindow *window, unsigned int VAO, unsigned int shaderProgram, unsigned int texture, unsigned int transformLoc, glm::vec3 cubePositions[], glm::vec3 *cameraPos, glm::vec3 *cameraDir){
    double time = glfwGetTime();

    glClearColor(std::sin(time),std::sin(time+(3.14/2)),std::cos(time),1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // std::cout << std::sin(time) << "   " << time << "\n";

    // camera location
    glm::mat4 view;
    view = glm::lookAt(*cameraPos, *cameraPos+*cameraDir, glm::vec3(0.0, 1.0, 0.0));

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
        std::cout << out[0] << "," << out[1] << "," << out[2] << " :" << i <<"\n";

        
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
    }
    glBindVertexArray(0); // i think this is the reason why glBindVertexArray(VAO) gets called again
}

void processInput(GLFWwindow *window, glm::vec3 *cameraPos, float *yaw, float *pitch)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    


    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        *cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        *cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        *cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        *cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        *yaw -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        *yaw += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        *pitch -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        *pitch += 0.1f;
    
    *yaw -= mouseVelX*0.005;
    *pitch -= mouseVelY*0.005;
}

