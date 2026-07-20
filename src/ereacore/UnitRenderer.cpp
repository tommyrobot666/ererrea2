#include <ereacore/UnitRenderer.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <3dListUtil.h>

#include <core/gameState.h>
#include <core/vertexObjectGenerators.h>

#include <ereacore/chunk.h>
#include <ereacore/Direction.h>

glm::vec3 UnitRenderer::unitFaces[] = {
    // 111 -> 010
    glm::vec3(1,1,1),
    glm::vec3(0,1,1),
    glm::vec3(1,1,0),
    glm::vec3(0,1,0),

    // 101 -> 000
    glm::vec3(1,0,1),
    glm::vec3(1,0,0),
    glm::vec3(0,0,1),
    glm::vec3(0,0,0),

    // 111 -> 100
    glm::vec3(1,1,1),
    glm::vec3(1,1,0),
    glm::vec3(1,0,1),
    glm::vec3(1,0,0),

    // 011 -> 000
    glm::vec3(0,1,1),
    glm::vec3(0,1,0),
    glm::vec3(0,0,1),
    glm::vec3(0,0,0),

    // 111 -> 001
    glm::vec3(1,1,1),
    glm::vec3(0,1,1),
    glm::vec3(1,0,1),
    glm::vec3(0,0,1),

    // 110 -> 000
    glm::vec3(1,1,0),
    glm::vec3(0,1,0),
    glm::vec3(1,0,0),
    glm::vec3(0,0,0),
};

UnitRenderer::~UnitRenderer() {
    Renderer::freeTexture(atlasTexture);
}

void UnitRenderer::render(std::vector<Chunk>& chunks, glm::mat4& proj) {
    gs.r().defaultShader();
    gs.r().currentTexture(atlasTexture);

    ListUtilVecInt cameraChunkPos = stepGridPos(gs.cameraPos.x,gs.cameraPos.y,gs.cameraPos.z,Chunk::LENGTH);
    for (auto& chunk : chunks) {
        if (dist(ListUtilVec{static_cast<double>(cameraChunkPos.x-chunk.x),
            static_cast<double>(cameraChunkPos.y-chunk.y),
            static_cast<double>(cameraChunkPos.z-chunk.z)}) > 2) continue;
        glm::mat4 chunkOffset = glm::translate(glm::mat4(1.0f),glm::vec3(chunk.x*Chunk::LENGTH,chunk.y*Chunk::LENGTH,chunk.z*Chunk::LENGTH));

        VertexObject* mesh = nullptr;
        for (auto& aMeshCacheEntry : chunkMeshCache) {
            if (aMeshCacheEntry.pos == glm::ivec3(chunk.x,chunk.y,chunk.z)) {
                if (chunk.queueMeshRegen) {
                    mesh = generateChunkMesh(chunk);
                    aMeshCacheEntry.mesh = mesh;
                }
                else {
                    mesh = aMeshCacheEntry.mesh;
                }
                break;
            }
        }
        if (mesh == nullptr) {
            mesh = generateChunkMesh(chunk);
            chunkMeshCache.push_back(meshCacheEntry{glm::ivec3(chunk.x,chunk.y,chunk.z),mesh});
        }
        chunk.queueMeshRegen = false;

        mesh->currentBind();
        glm::mat4 trans = proj*gs.view*chunkOffset;
        gs.r().setShaderTransform(&trans);
        mesh->draw();
    }
}

// TODO move to helper class
void UnitRenderer::getOrAddVertex(std::vector<UnitRenderer::Vertex> &vertices, UnitRenderer::Vertex &v, unsigned int &i) {
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
    std::vector<unsigned int> indices;

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

                glm::ivec3 pos = glm::vec3(x,y,z);
                for (int i = 0; i < Direction::ALL_SIZE; i++) {
                    glm::ivec3 direction = Direction::ALL_VEC[i];
                    auto neighbor = pos+direction;
                    int unitFacesStart = i * 4;
                    if (Chunk::inBounds(neighbor.x,neighbor.y,neighbor.z)) {
                        if (chunk.getUnit(neighbor.x,neighbor.y,neighbor.z) != Unit::NONE) continue;
                    }

                    auto v0 = Vertex{unitFaces[unitFacesStart]+static_cast<glm::vec3>(pos),
                        glm::vec2{atlasCords.x,atlasCords.y}};
                    unsigned int i0;
                    getOrAddVertex(vertices, v0, i0);

                    auto v1 = Vertex{unitFaces[unitFacesStart+1]+static_cast<glm::vec3>(pos),
                        glm::vec2{atlasCords.x,atlasCords.w}};
                    unsigned int i1;
                    getOrAddVertex(vertices,v1,i1);

                    auto v2 = Vertex{unitFaces[unitFacesStart+2]+static_cast<glm::vec3>(pos),
                        glm::vec2{atlasCords.z,atlasCords.y}};
                    unsigned int i2;
                    getOrAddVertex(vertices,v2,i2);

                    auto v3 = Vertex{unitFaces[unitFacesStart+3]+static_cast<glm::vec3>(pos),
                        glm::vec2{atlasCords.z,atlasCords.w}};
                    unsigned int i3;
                    getOrAddVertex(vertices,v3,i3);
                    // std::cout << i3 <<"is"<< v3.pos.x<<v3.pos.y<<v3.pos.z<<"\n";

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

    // TODO move to helper class
    std::vector<float> verticesFloats[vertexObjectGenerators::floatsInVertex*vertices.size()];
    for (Vertex & vertex : vertices) {
        verticesFloats->push_back(vertex.pos.x);
        verticesFloats->push_back(vertex.pos.y);
        verticesFloats->push_back(vertex.pos.z);
        verticesFloats->push_back(1.f);
        verticesFloats->push_back(1.f);
        verticesFloats->push_back(1.f);
        verticesFloats->push_back(vertex.uv.x);
        verticesFloats->push_back(vertex.uv.y);
    }

    return Renderer::createVertexObject(
        verticesFloats->data(),
        indices.data(),
        vertices.size()*vertexObjectGenerators::SizeOfVertex,
        indices.size()*sizeof(int));
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

    unsigned char* grassTexture = Renderer::loadPngData("grass.png");
    unsigned char* dirtTexture = Renderer::loadPngData("dort.png");
    unsigned char* stoneTexture = Renderer::loadPngData("stone.png");
    unsigned char* oreTexture = Renderer::loadPngData("ore.png");
    unsigned char** textures = new unsigned char*[]{grassTexture,dirtTexture,stoneTexture,oreTexture};
    atlasTexture = Renderer::createTextureAtlas(textures,4,16);
    Renderer::textureDrawingNearest(); // texture settings only apply to bound texture
    free(grassTexture);
    free(dirtTexture);
    free(stoneTexture);
    free(oreTexture);
    free(textures);

    loaded = true;
}
