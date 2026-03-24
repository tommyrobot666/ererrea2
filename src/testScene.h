#include <gameState.h>
#include <scene.h>
#include <renderer.h>
#include <vertexObjectGenerators.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600

class testScene:scene
{
private:
    inline static const glm::vec3 cubePositions[];
    unsigned int texture; // use the new render class
    unsigned int VAO; // use the new render class
public:
    void load();
    void simulate();
    void render();
};