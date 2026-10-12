#pragma once

#include <core/renderer.h>

class Sprites {
    unsigned int texture;
    unsigned int sideLength;
    unsigned int spriteLength;
    unsigned int totalAtlasSize;
    std::vector<VertexObject*> quads;
    std::vector<glm::vec4> spriteUV;

public:
    ~Sprites();
    // generate full grid atlas
    Sprites(unsigned int texture, unsigned int sideLength, unsigned int spriteLength);
    // manually add non-square sprites
    Sprites(unsigned int texture, unsigned int textureLength);
    glm::vec4 UVRegion(unsigned int x,unsigned int y,unsigned int width,unsigned int height) const;
    VertexObject* getSprite(unsigned int idx);
    void drawSprite(unsigned int idx);
    void drawSpriteAt(unsigned int idx, glm::mat4& t);
    void draw(unsigned int idx,float x,float y,float width,float height,glm::mat4& orthProj);
    unsigned int addSprite(unsigned int x,unsigned int y,unsigned int width,unsigned int height);
};

namespace OrthProjManager {
    enum Anchor {
        TOP_LEFT,
        BOTTOM_LEFT
    };

    extern glm::mat4* orthProjs;

    glm::mat4& getOrthProjForAnchor(Anchor a);
    void updateOrthProjs(float screenWidth, float screenHeight); // set in startFrame
};
