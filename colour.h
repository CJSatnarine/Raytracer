#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"
#include "vec3.h"

using colour = vec3;

inline double linearToGamma(double linearComponent) {
    if (linearComponent > 0) return sqrt(linearComponent);

    return 0;
}

void writeColour(std::ostream& out, const colour& pixelColour) {
    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();

    // Apply a linear to gamma transform for gamma 2. 
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Translate the [0, 1] component values to the byte range [0, 255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(255 * intensity.clamp(r));
    int gbyte = int(255 * intensity.clamp(g));
    int bbyte = int(255 * intensity.clamp(b));

    // Write out the pixel colour components. 
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif