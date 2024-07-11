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
        double u;
        double v;
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

class translate : public hittable {
    public:
        translate(shared_ptr<hittable> object, const vec3& offset) : object(object), offset(offset) {}

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            // Move the ray backwards by the offset.
            ray offsetR(r.origin() - offset, r.direction(), r.time());

            // Determine whether an intersection exists along the offset ray (and if so, where)
            if (!object->hit(offsetR, rayT, rec))
                return false;

            // Move the intersection point forwards by the offset
            rec.p += offset;

            return true;
        }

        aabb boundingBox() const override {
            return bBox;
        }

    private: 
        shared_ptr<hittable> object;
        vec3 offset;
        aabb bBox;
};

#endif