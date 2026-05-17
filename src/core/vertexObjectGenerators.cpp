#include <core/vertexObjectGenerators.h>

#define f1 1.0f
#define f0 0.0f

namespace vertexObjectGenerators {
    namespace basicCube {
        float *vertices() {
            return new float[288]{
                // positions           // colors       // uvs
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
            };
        }

        int verticesSize() {
            return 36;
        }
    }

     namespace cube {
        float* vertices(){
            return new float[8*floatsInVertex]{
                //pos     color      uv
                // 1f,1f,1f, 1f,1f,1f, 1f,1f,
                // 0f,1f,1f, 1f,1f,1f, 0f,1f,
                // 1f,0f,1f, 1f,1f,1f, 1f,0f,
                // 0f,0f,1f, 1f,1f,1f, 0f,0f,
                //
                // 1f,1f,0f, 1f,1f,1f, 1f,1f,
                // 0f,1f,0f, 1f,1f,1f, 0f,1f,
                // 1f,0f,0f, 1f,1f,1f, 1f,0f,
                // 0f,0f,0f, 1f,1f,1f, 0f,0f
                
                f1,f1,f1, f1,f1,f1, f1,f1,
                f0,f1,f1, f1,f1,f1, f0,f1,
                f1,f0,f1, f1,f1,f1, f1,f0,
                f0,f0,f1, f1,f1,f1, f0,f0,

                f1,f1,f0, f1,f1,f1, f1,f1,
                f0,f1,f0, f1,f1,f1, f0,f1,
                f1,f0,f0, f1,f1,f1, f1,f0,
                f0,f0,f0, f1,f1,f1, f0,f0
            };
        }
        int* indices(){
            return new int[]{
                // 111 -> 001
                0,1,2,
                2,1,3,
                // 111 -> 010
                0,4,1,
                1,4,5,
                // 111 -> 100
                0,2,4,
                4,2,6,

                // 110 -> 000
                4,5,6,
                6,5,7,
                // 000 -> 011
                7,5,3,
                3,5,1,
                // 000 -> 101
                7,6,3,
                3,6,2
            };
        }
        int verticeCount() {return 8;}
        int indiceCount() {return 36;}
    }

    namespace quad {
        int *indices() {
            return new int[6]{
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
            };
        }

        float *vertices() {
            return new float[32]{
                // positions           // colors       // uvs
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,// top left
            };
        }

        float *verticesPositions() {
            return new float[12]{
                0.5f, 0.5f, 0.0f, // top right
                0.5f, -0.5f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f // top left
            };
        }

        int size() {
            return 4;
        }
    }
}
