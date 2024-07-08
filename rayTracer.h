#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
//#include <random>

// C++ std usings. 
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants. 
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions. 
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble(void) {
    // Returns a random real in [0, 1). 
    return rand() / (RAND_MAX + 1.0);
}

// The same as randomDouble() but using the random include. 
// inline double randomDouble(void) {
//     static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//     static std::mt19937 generator;
//     return distribution(generator);
// }

inline double randomDouble(double min, double max) {
    // Returns a random real in [min, max).
    return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
    // Returns a random integer in [min, max].
    return int(randomDouble(min, max++));
}

// Common headers. 
#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif