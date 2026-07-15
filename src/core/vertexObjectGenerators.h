#pragma once
namespace vertexObjectGenerators {
    static constexpr int floatsInVertex = 8;
    static constexpr int SizeOfVertex = floatsInVertex*sizeof(float);

    namespace basicCube {
        float *vertices();

        int verticesSize();
    }

    namespace cube {
        float* vertices();
        int* indices();
        int verticeCount();
        int indiceCount();
    }

    namespace quad {
        int *indices();
        float *vertices();
        float *verticesPositions();
        int size();
    }
}
