#include <ereacore/ereaGameScene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <3dListUtil.h>
#include <core/vertexObjectGenerators.h>
#include <core/gameState.h>

void ereaGameScene::load() {
    // chunks.push_back(Chunk(0,0,0));
    chunks.emplace_back(0,0,0);

    float* vertices = vertexObjectGenerators::cube::vertices();
    int *indices = vertexObjectGenerators::cube::indices();
    cubeModel = Renderer::createVertexObject(
        vertices,
        reinterpret_cast<unsigned int*>(indices),
        vertexObjectGenerators::cube::verticeCount()*vertexObjectGenerators::SizeOfVertex,
        vertexObjectGenerators::cube::indiceCount()*sizeof(int)
    );
    free(vertices);
    free(indices);

    gs.r().defaultShader();
}

void ereaGameScene::simulate() {

}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    for (auto chunk : chunks) {
        glm::mat4 chunkOffset = glm::translate(glm::mat4(1.0f),glm::vec3(chunk.x*Chunk::LENGTH,chunk.y*Chunk::LENGTH,chunk.z*Chunk::LENGTH));

        for (int x = 0; x < Chunk::LENGTH; ++x){
            for (int y = 0; y <= Chunk::LENGTH; ++y) {
                for (int z = 0; z <= Chunk::LENGTH; ++z) {
                    if (chunk.units[posToIdx(x,y,z,Chunk::LENGTH)] == Unit::NONE) continue;

                    // Renderer::currentTexture(texture); get unit texture

                    glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(x,y,z));
                    trans = trans*chunkOffset*proj*gs.view;
                    gs.r().setShaderTransform(&trans);
                    cubeModel->draw();
                }
            }
        }
    }
}

void ereaGameScene::close() {
    delete(cubeModel);
    // delete(chunks);
}

