#include <core/d2renderer.h>

#include <glm/gtc/type_ptr.hpp>

Sprites::~Sprites() {
    Renderer::freeTexture(texture);
}

Sprites::Sprites(unsigned int texture, unsigned int sideLength, unsigned int spriteLength)
: texture(texture), sideLength(sideLength), spriteLength(spriteLength), totalAtlasSize(sideLength*spriteLength) {
    assert(texture > 0);
    assert(sideLength > 0);
    assert(spriteLength > 0);

    float offset = 1/sideLength;
    float x = 0, y = 0;
    for (int i = 0; i < sideLength; ++i) {
        for (int j = 0; j < sideLength; ++j) {
            spriteUV.emplace_back(x,y,x+offset,y+offset);

            x += offset;
        }
        y += offset;
    }

    VertexList baseQuad;
    baseQuad.addTriangleNoRef(
        Vertex{glm::vec3{0,0,0},glm::vec2{0,0}},
        Vertex{glm::vec3{-1,0,0},glm::vec2{1,0}},
        Vertex{glm::vec3{0,-1,0},glm::vec2{0,1}}
        );
    baseQuad.addTriangleNoRef(
        Vertex{glm::vec3{-1,-1,0},glm::vec2{1,1}},
        Vertex{glm::vec3{-1,0,0},glm::vec2{1,0}},
        Vertex{glm::vec3{0,-1,0},glm::vec2{0,1}}
        );

    for (auto& uv : spriteUV) {
        VertexList thisQuad = VertexList(baseQuad);
        thisQuad.scaleAndTransformUV(0,0,offset,offset);
        thisQuad.scaleAndTransformUV(uv.x,uv.y,1,1);
        quads.push_back(thisQuad.makeObject());
    }
}

Sprites::Sprites(unsigned int texture, unsigned int textureLength)
: texture(texture), sideLength(1), spriteLength(textureLength), totalAtlasSize(textureLength) {
    assert(texture > 0);
    assert(textureLength > 0);
}

glm::vec4 Sprites::UVRegion(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const {
    assert(width > 0);
    assert(height > 0);

    float uvX = static_cast<float>(x)/static_cast<float>(totalAtlasSize);
    float uvY = static_cast<float>(y)/static_cast<float>(totalAtlasSize);
    return {uvX, uvY, uvX+static_cast<float>(width)/static_cast<float>(totalAtlasSize), uvY+static_cast<float>(height)/static_cast<float>(totalAtlasSize)};
}

VertexObject* Sprites::getSprite(unsigned int idx) {
    assert(quads.size() > idx);

    return quads[idx];
}

void Sprites::drawSprite(unsigned int idx) {
    assert(quads.size() > idx);

    if (renderer.d2texture0 != texture) {
        Renderer::currentTexture(texture);
    }

    getSprite(idx)->bindAndDraw();
}

void Sprites::drawSpriteAt(unsigned int idx, glm::mat4 &t) {
    assert(quads.size() > idx);

    if (renderer.d2texture0 != texture) {
        Renderer::currentTexture(texture);
    }

    getSprite(idx)->bindAndDrawAt(t);
}

void Sprites::draw(unsigned int idx, float x, float y, float width, float height, glm::mat4& orthProj) {
    assert(quads.size() > idx);

    if (renderer.d2texture0 != texture) {
        Renderer::currentTexture(texture);
    }
    auto trans = glm::mat4(1.0f);
    trans = glm::translate(trans,glm::vec3(x,y,0));
    trans = glm::scale(trans, glm::vec3(width,height,1));
    trans = orthProj*trans;
    trans = glm::scale(trans, glm::vec3(-1,-1,1));
    drawSpriteAt(idx,trans);
}

unsigned int Sprites::addSprite(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    assert(width > 0);
    assert(height > 0);

    auto uv = UVRegion(x, y, width, height);
    VertexList quad;
    quad.addTriangleNoRef(
        Vertex{glm::vec3{0,0,0},glm::vec2{uv.x,uv.y}},
        Vertex{glm::vec3{-1,0,0},glm::vec2{uv.z,uv.y}},
        Vertex{glm::vec3{0,-1,0},glm::vec2{uv.x,uv.w}}
        );
    quad.addTriangleNoRef(
        Vertex{glm::vec3{-1,-1,0},glm::vec2{uv.x,uv.y}},
        Vertex{glm::vec3{-1,0,0},glm::vec2{uv.z,uv.y}},
        Vertex{glm::vec3{0,-1,0},glm::vec2{uv.x,uv.w}}
        );
    quads.emplace_back(quad.makeObject());
    spriteUV.emplace_back(uv);
    return quads.size()-1;
}

glm::mat4* OrthProjManager::orthProjs = new glm::mat4[2] {
    glm::mat4(),glm::mat4(),
};

glm::mat4 & OrthProjManager::getOrthProjForAnchor(Anchor a) {
    return orthProjs[a];
}

void OrthProjManager::updateOrthProjs(float screenWidth, float screenHeight) {
    assert(screenWidth > 0);
    assert(screenHeight > 0);

    orthProjs[TOP_LEFT] = glm::ortho(0.f,screenWidth,screenHeight,0.f);
    orthProjs[BOTTOM_LEFT] = glm::ortho(0.f,screenWidth,0.f,screenHeight);
}
