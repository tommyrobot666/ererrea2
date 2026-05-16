#pragma once
namespace vertexObjectGenerators {
    static constexpr int floatsInVertex = 8;
    static constexpr int SizeOfVertex = floatsInVertex*sizeof(float);

    namespace basicCube {
        float *vertices();

        int verticesSize();
    }

    namespace quad {
        int *indices();

        float *vertices();

        int size();
    }
}
