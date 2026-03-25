#pragma once
namespace vertexObjectGenerators {
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
