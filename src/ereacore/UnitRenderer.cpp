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

void UnitRenderer::get_or_add_vertex(std::vector<UnitRenderer::Vertex> vertices, UnitRenderer::Vertex v, int &i) {
    bool alreadyExists = false;
    for (int j = 0; j < vertices.size(); ++j) {
        auto vertex = vertices[j];
        if (v == vertex) {
            alreadyExists = true;
            i = j;
        }
    }
    if (!alreadyExists) {
        i = vertices.size();
        vertices.push_back(v);
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
                for (int i = 0; i < 3; i++) {
                    glm::vec3 direction = Direction::ALL_VEC[i];
                    auto neighbor = pos+direction;
                    int unitFacesStart = i * vertexObjectGenerators::floatsInVertex * 4;
                    // TODO don't get units out of bounds
                    if (chunk.getUnit(neighbor.x,neighbor.y,neighbor.z) != Unit::NONE) continue;

                    auto v0 = Vertex{unitFaces[unitFacesStart],glm::vec2{atlasCords.x,atlasCords.y}};
                    int i0;
                    get_or_add_vertex(vertices, v0, i0);

                    auto v1 = Vertex{unitFaces[unitFacesStart+vertexObjectGenerators::floatsInVertex],glm::vec2{atlasCords.x,atlasCords.w}};
                    int i1;
                    get_or_add_vertex(vertices,v1,i1);

                    auto v2 = Vertex{unitFaces[unitFacesStart+vertexObjectGenerators::floatsInVertex*2],glm::vec2{atlasCords.z,atlasCords.y}};
                    int i2;
                    get_or_add_vertex(vertices,v2,i2);

                    auto v3 = Vertex{unitFaces[unitFacesStart+vertexObjectGenerators::floatsInVertex*3],glm::vec2{atlasCords.z,atlasCords.w}};
                    int i3;
                    get_or_add_vertex(vertices,v3,i3);

                    //t1
                    indices.push_back(i0);
                    indices.push_back(i1);
                    indices.push_back(i2);
                    //t2
                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i3);
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
