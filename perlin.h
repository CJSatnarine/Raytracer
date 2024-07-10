#ifndef PERLIN_H
#define PERLIN_H

#include "rayTracer.h"

class perlin {
    public: 
        perlin() {
            randVector = new vec3[pointCount];
            for (int i = 0; i < pointCount; i++) {
                randVector[i] = unitVector(vec3::random(-1, 1));
            }

            permX = perlinGeneratePerm();
            permY = perlinGeneratePerm();
            permZ = perlinGeneratePerm();
        }

        ~perlin() {
            delete[] randVector;
            delete[] permX;
            delete[] permY;
            delete[] permZ;
        }

        double noise(const point3& p) const {
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            auto i = int(floor(p.x()));
            auto j = int(floor(p.y()));
            auto k = int(floor(p.z()));
            vec3 c[2][2][2];

            for (int di = 0; di < 2; di++) {
                for (int dj = 0; dj < 2; dj++) {
                    for (int dk = 0; dk < 2; dk++) {
                        c[di][dj][dk] = randVector[
                            permX[(i+di) & 255] ^
                            permY[(j+dj) & 255] ^
                            permZ[(k+dk) & 255]
                        ];
                    }
                }
            }
            return perlinInterpolation(c, u, v, w);
        }

        double turbulance(const point3& p, int depth) const {
            auto accum = 0.0;
            auto temporaryP = p;
            auto weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(temporaryP);
                weight *= 0.5;
                temporaryP *= 2;
            }

            return fabs(accum);
        }

    private: 
        static const int pointCount = 256;
        vec3* randVector;
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

        static double perlinInterpolation(const vec3 c[2][2][2], double u, double v, double w) {
            auto uu = u * u * (3 - 2 * u);
            auto vv = v * v * (3 - 2 * v);
            auto ww = w * w * (3 - 2 * w);
            auto accum = 0.0;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++) {
                        vec3 weightV(u - i, v - j, w - k);
                        accum += (i * uu + (1 - i)*(1 - uu))
                            * (j * vv + (1 - j) * (1 - vv))
                            * (k * ww + (1 - k) * (1 - ww))
                            * dot(c[i][j][k], weightV);
                    }

            return accum;
        }
};

#endif