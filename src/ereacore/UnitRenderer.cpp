#include <ereacore/UnitRenderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <3dListUtil.h>

#include <core/gameState.h>
#include <core/vertexObjectGenerators.h>

#include <ereacore/chunk.h>
#include <ereacore/Direction.h>

UnitRenderer::~UnitRenderer() {
    Renderer::freeTexture(grassTexture);
    Renderer::freeTexture(dirtTexture);
    Renderer::freeTexture(stoneTexture);
    Renderer::freeTexture(oreTexture);
}

void UnitRenderer::render(std::vector<Chunk>& chunks, glm::mat4& proj) {
    gs.r().defaultShader();
    cubeModel->currentBind();

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
                    cubeModel->draw();
                }
            }
        }
    }
}

VertexObject* UnitRenderer::generateChunkMesh(Chunk &chunk) {
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    for (int x = 0; x < Chunk::LENGTH; ++x){
        for (int y = 0; y < Chunk::LENGTH; ++y) {
            for (int z = 0; z < Chunk::LENGTH; ++z) {
                if (chunk.units[posToIdx(x,y,z,Chunk::LENGTH)] == Unit::NONE) continue;

                glm::vec4 atlasCords;
                switch (chunk.units[posToIdx(x,y,z,Chunk::LENGTH)])
                {
                    case NONE:
                        throw;
                    case GRASS:
                        atlasCords = glm::vec4{0,0,1/2,1/2};
                        break;
                    case DIRT:
                        atlasCords = glm::vec4{1/2,0,1,1/2};
                        break;
                    case STONE:
                        atlasCords = glm::vec4{0,1/2,1/2,1};
                        break;
                    case ORE:
                        atlasCords = glm::vec4{1/2,1/2,1,1};
                        break;
                }

                glm::vec3 pos = glm::vec3(x,y,z);
                int i = -1;
                for (glm::vec3 direction : Direction::ALL_VEC) {
                    i++;
                    auto neighbor = pos+direction;
                    // TODO don't get units out of bounds
                    if (chunk.getUnit(neighbor.x,neighbor.y,neighbor.z) != Unit::NONE) continue;
                    auto v0 = Vertex{unitFaces0[i],glm::vec2{0,0},atlasCords};
                    int i0;
                    bool v0AlreadyExists = false;
                    for (int j = 0; j < vertices.size(); ++j) {
                        auto vertex = vertices[j];
                        if (v0 == vertex) {
                            v0AlreadyExists = true;
                            i0 = j;
                        }
                    }
                    if (!v0AlreadyExists) {
                        i0 = vertices.size();
                        vertices.push_back(v0);
                    }

                    auto v1 = Vertex{unitFaces1[i],glm::vec2{1,1},atlasCords};
                    int i1;
                    bool v1AlreadyExists = false;
                    for (int j = 0; j < vertices.size(); ++j) {
                        auto vertex = vertices[j];
                        if (v1 == vertex) {
                            v1AlreadyExists = true;
                            i1 = j;
                        }
                    }
                    if (!v1AlreadyExists) {
                        i1 = vertices.size();
                        vertices.push_back(v1);
                    }

                    indices.push_back(i0);
                    indices.push_back(i1);
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
