#ifndef HITTABLE_H
#define HITTABLE_H

#include "rayTracer.h"
#include "aabb.h"

class material;

class hitRecord {
    public: 
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool frontFace;

        void setFaceNormal(const ray& r, const vec3& outwardNormal) {
            /* 
            * Sets the hit record normal vector.
            * NOTE: the parameter `outward_normal` is assumed to have unit length.
            */
           
            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
};

class hittable {
    public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval rayT, hitRecord& rec) const = 0; 

    virtual aabb boundingBox() const = 0;
};

#endif