#ifndef AABB_H
#define AABB_H

#include "rayTracer.h"

class aabb {
  public:
    interval x, y, z;

    aabb() {} // The default AABB is empty, since intervals are empty by default.

    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

    aabb(const point3& a, const point3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
    }

    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axisInterval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval rayT) const {
        const point3& rayOrigin = r.origin();
        const vec3& rayDirection  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval& ax = axisInterval(axis);
            const double adInv = 1.0 / rayDirection[axis];

            auto t0 = (ax.min - rayOrigin[axis]) * adInv;
            auto t1 = (ax.max - rayOrigin[axis]) * adInv;

            if (t0 < t1) {
                if (t0 > rayT.min) rayT.min = t0;
                if (t1 < rayT.max) rayT.max = t1;
            } else {
                if (t1 > rayT.min) rayT.min = t1;
                if (t0 < rayT.max) rayT.max = t0;
            }

            if (rayT.max <= rayT.min) return false;
        }
        return true;
    }
};

#endif