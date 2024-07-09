#ifndef PERLIN_H
#define PERLIN_H

#include "rayTracer.h"

class perlin {
    public: 
        perlin() {
            randFloat = new double[pointCount];
            for (int i = 0; i < pointCount; i++) {
                randFloat[i] = randomDouble();
            }

            permX = perlinGeneratePerm();
            permY = perlinGeneratePerm();
            permZ = perlinGeneratePerm();
        }

        ~perlin() {
            delete[] randFloat;
            delete[] permX;
            delete[] permY;
            delete[] permZ;
        }

        double noise(const point3& p) const {
            auto i = int(4 * p.x()) & 255;
            auto j = int(4 * p.y()) & 255;
            auto k = int(4 * p.z()) & 255;

            return randFloat[permX[i] ^ permY[j] ^ permZ[k]];
        }

    private: 
        static const int pointCount = 256;
        double* randFloat;
        int* permX;
        int* permY;
        int* permZ;

        static int* perlinGeneratePerm(void) {
            auto p = new int[pointCount];

            for (int i = 0; i < pointCount; i++) {
                p[i] = i;
            }

            permute(p, pointCount);

            return p;
        }

        static void permute(int* p, int n) {
            for (int i = n - 1; i > 0; i--) {
                int target = randomInt(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
};

#endif