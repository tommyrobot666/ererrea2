#include <ereacore/ereaGameScene.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <3dListUtil.h>
#include <core/vertexObjectGenerators.h>
#include <core/gameState.h>

void ereaGameScene::load() {
    auto chunk1 = Chunk(0,0,0);
    chunk1.fillUnits(0,7,0,16,8,16,Unit::GRASS);
    chunk1.fillUnits(0,5,0,16,7,16,Unit::DIRT);
    chunk1.fillUnits(0,0,0,16,5,16,Unit::STONE);
    chunk1.fillUnits(0,0,0,8,5,8,Unit::ORE);

    chunks.push_back(chunk1);

    chunks.emplace_back(1,0,0);
    auto& chunk2 = chunks[1]; // use & instead of *
    chunk2.fillUnits(0,7,0,16,8,16,Unit::GRASS);
    chunk2.fillUnits(0,5,0,16,7,16,Unit::DIRT);
    chunk2.fillUnits(0,0,0,16,5,16,Unit::STONE);
    chunk2.fillUnits(0,0,0,8,5,8,Unit::ORE);
    // dirtTexture = Renderer::loadPngTextureNearest("dort.png");

    // chunkGenerator.debugTex = dirtTexture;
    // chunkGenerator.debugtex();
    //
    // chunkGenerator.seed = 123;
    //
    // for (int x = 0; x < 3; x++) {
    //     for (int y = 0; y < 1; y++) {
    //         for (int z = 0; z < 3; z++) {
    //             chunks.emplace_back(x,y,z);
    //             auto& chunk = chunks.back();
    //             chunkGenerator.generateChunk(chunk);
    //         }
    //     }
    // }


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

    grassTexture = Renderer::loadPngTexture("grass.png");
    Renderer::textureDrawingNearest(); // texture settings only apply to bound texture
    dirtTexture = Renderer::loadPngTextureNearest("dort.png");
    stoneTexture = Renderer::loadPngTextureNearest("stone.png");
    oreTexture = Renderer::loadPngTextureNearest("ore.png");

    gs.r().defaultShader();
    cubeModel->currentBind();

    gs.cameraPos = glm::vec3(0,9,0);
}

void moveCamera() {
    float sprint = 1.0f;
    if (glfwGetKey(gs.window, GLFW_KEY_LEFT_SHIFT))
        sprint = 68.0f;

    if (glfwGetKey(gs.window, GLFW_KEY_LEFT) == GLFW_PRESS)
        gs.yaw -= 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        gs.yaw += 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_DOWN) == GLFW_PRESS)
        gs.pitch -= 1.3f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_UP) == GLFW_PRESS)
        gs.pitch += 1.3f*gs.deltaTime*sprint;

    if (glfwGetKey(gs.window, GLFW_KEY_W) == GLFW_PRESS)
        gs.cameraPos.z += 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_S) == GLFW_PRESS)
        gs.cameraPos.z -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_A) == GLFW_PRESS)
        gs.cameraPos.x -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_D) == GLFW_PRESS)
        gs.cameraPos.x += 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_Q) == GLFW_PRESS)
        gs.cameraPos.y -= 2.7f*gs.deltaTime*sprint;
    if (glfwGetKey(gs.window, GLFW_KEY_E) == GLFW_PRESS)
        gs.cameraPos.y += 2.7f*gs.deltaTime*sprint;

    if(glfwGetKey(gs.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gs.window, true);
}

void generateChunk(Chunk& chunk) {
    if (chunk.y > 0) {
        // just air
        return;
    }
    if (chunk.y < 0) {
        chunk.fillUnits(0,0,0,16,16,16,Unit::STONE);
        chunk.fillUnits(0,2,-8,8,7,0,Unit::ORE);
        return;
    }
    // else
    chunk.fillUnits(0,7,0,16,8,16,Unit::GRASS);
    chunk.fillUnits(0,5,0,16,7,16,Unit::DIRT);
    chunk.fillUnits(0,0,0,16,5,16,Unit::STONE);
    chunk.fillUnits(0,0,0,8,5,8,Unit::ORE);
}

void ereaGameScene::generateNearbyChunks() {
    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    int maxChunkDistance = 2;
    for (int x = cameraChunkPos.x-maxChunkDistance; x < cameraChunkPos.x+maxChunkDistance; ++x) {
        for (int y = cameraChunkPos.y-maxChunkDistance; y < cameraChunkPos.y+maxChunkDistance; ++y) {
            for (int z = cameraChunkPos.z-maxChunkDistance; z < cameraChunkPos.z+maxChunkDistance; ++z) {
                Chunk* isChunk = Chunk::findChunkOrNone(chunks,x,y,z);
                if (isChunk != nullptr) continue;
                chunks.emplace_back(x,y,z);
                auto& chunk = chunks.back();
                generateChunk(chunk);
            }
        }
    }
}

void ereaGameScene::interactWithUnits() {
    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    int maxChunkDistance = 2;
    for (int x = cameraChunkPos.x-maxChunkDistance; x < cameraChunkPos.x+maxChunkDistance; ++x) {
        for (int y = cameraChunkPos.y-maxChunkDistance; y < cameraChunkPos.y+maxChunkDistance; ++y) {
            for (int z = cameraChunkPos.z-maxChunkDistance; z < cameraChunkPos.z+maxChunkDistance; ++z) {
                Chunk* chunk = Chunk::findChunkOrNone(chunks,x,y,z);
                if (chunk == nullptr) continue;
                for (int xx = 0; xx < Chunk::LENGTH; ++xx) {
                    for (int yy = 0; yy < Chunk::LENGTH; ++yy) {
                        for (int zz = 0; zz < Chunk::LENGTH; ++zz) {
                            // for every unit in nearby chunks

                            if (dist(ListUtilVec{xx+x*Chunk::LENGTH-gs.cameraPos.x,
                                    yy+y*Chunk::LENGTH-gs.cameraPos.y,
                                    zz+z*Chunk::LENGTH-gs.cameraPos.z}) < 10) {
                                if (chunk->units[posToIdx(xx,yy,zz,Chunk::LENGTH)] == Unit::GRASS
                                    || chunk->units[posToIdx(xx,yy,zz,Chunk::LENGTH)] == Unit::DIRT) {
                                    chunk->setUnit(xx,yy,zz,Unit::DIRT);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ereaGameScene::simulate() {
    moveCamera();
    generateNearbyChunks();
    interactWithUnits();
}

void ereaGameScene::render() {
    Renderer::clear(1.0f,1.0f,1.0f,1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)GAME_WINDOW_WIDTH/(float)GAME_WINDOW_HEIGHT, 0.1f, 100.0f);

    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    for (auto& chunk : chunks) {
        if (dist(ListUtilVec{static_cast<double>(cameraChunkPos.x-chunk.x),
            static_cast<double>(cameraChunkPos.y-chunk.y),
            static_cast<double>(cameraChunkPos.z-chunk.z)}) > 1) continue;

        glm::mat4 chunkOffset = glm::translate(glm::mat4(1.0f),glm::vec3(chunk.x*Chunk::LENGTH,chunk.y*Chunk::LENGTH,chunk.z*Chunk::LENGTH));

        for (int x = 0; x < Chunk::LENGTH; ++x){
            for (int y = 0; y < Chunk::LENGTH; ++y) {
                for (int z = 0; z < Chunk::LENGTH; ++z) {
                    if (chunk.units[posToIdx(x,y,z,Chunk::LENGTH)] == Unit::NONE) continue;


                    unsigned int texture;
                    switch (chunk.units[posToIdx(x,y,z,Chunk::LENGTH)])
                    {
                        case NONE:
                            throw;
                        case GRASS:
                            texture = grassTexture;
                            break;
                        case DIRT:
                            texture = dirtTexture;
                            break;
                        case STONE:
                            texture = stoneTexture;
                            break;
                        case ORE:
                            texture = oreTexture;
                            break;
                    }
                    Renderer::currentTexture(texture);

                    glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(x,y,z));
                    trans = proj*gs.view*trans*chunkOffset;
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
    Renderer::freeTexture(grassTexture);
    Renderer::freeTexture(dirtTexture);
    Renderer::freeTexture(stoneTexture);
    Renderer::freeTexture(oreTexture);
}

