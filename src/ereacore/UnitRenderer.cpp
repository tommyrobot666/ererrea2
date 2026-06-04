#include <ereacore/UnitRenderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <3dListUtil.h>

#include <core/gameState.h>
#include <core/vertexObjectGenerators.h>

#include <ereacore/chunk.h>

UnitRenderer::~UnitRenderer() {
    Renderer::freeTexture(grassTexture);
    Renderer::freeTexture(dirtTexture);
    Renderer::freeTexture(stoneTexture);
    Renderer::freeTexture(oreTexture);
}

void UnitRenderer::render(std::vector<Chunk>& chunks, glm::mat4& proj) {
    gs.r().defaultShader();
    cubeModel.currentBind();

    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    for (auto& chunk : chunks) {
        if (dist(ListUtilVec{static_cast<double>(cameraChunkPos.x-chunk.x),
            static_cast<double>(cameraChunkPos.y-chunk.y),
            static_cast<double>(cameraChunkPos.z-chunk.z)}) > 2) continue;

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
                    cubeModel.draw();
                }
            }
        }
    }
}

void UnitRenderer::load() {
    assert(!loaded);

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

    loaded = true;
}
