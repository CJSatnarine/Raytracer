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

class rotateY : public hittable {
    public: 
        rotateY(shared_ptr<hittable> object, double angle) : object(object) {
            auto radians = degreesToRadians(angle);
            sinTheta = sin(radians);
            cosTheta = cos(radians);
            bBox = object->boundingBox();

            point3 min(infinity, infinity, infinity);
            point3 max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; i ++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i * bBox.x.max + (1 - i) * bBox.x.min;
                        auto y = j * bBox.y.max + (1 - j) * bBox.y.min;
                        auto z = k * bBox.z.max + (1 - k) * bBox.z.min;

                        auto newX = cosTheta * x + sinTheta * z;
                        auto newZ = -sinTheta * x + cosTheta * z;

                        vec3 tester(newX, y, newZ);

                        for (int c = 0; c < 3; c++) {
                            min[c] = fmin(min[c], tester[c]);
                            max[c] = fmax(max[c], tester[c]);
                        }
                    }  
                }
            }

            bBox = aabb(min, max);
        }

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            // Change the ray from world space to object space. 
            auto origin = r.origin();
            auto direction = r.direction();

            origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
            origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];


            direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
            direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];
        
            ray rotatedR(origin, direction, r.time());

            // Determine whether an intersection exists in object space (and if so, where). 
            if (!object->hit(rotatedR, rayT, rec)) return false;

            // Change the intersection point from object space to world space. 
            auto p = rec.p;
            p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
            p[2] = -sinTheta * rec.p[0] + cosTheta * rec.normal[2];

            // Change the normal from object space to world space. 
            auto normal = rec.normal;
            normal[0] =  cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
            normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

            rec.p = p;
            rec.normal = normal;

            return true;
        }

    private: 
        shared_ptr<hittable> object;
        double sinTheta;
        double cosTheta;
        aabb bBox;
};

#endif