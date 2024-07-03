#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rayTracer.h"

class sphere : public hittable {
    private:
        point3 centre;
        double radius;
        shared_ptr<material> mat;

    public: 
        sphere(const point3& centre, double radius, shared_ptr<material> mat)
        : centre(centre), radius(fmax(0,radius)), mat(mat) {}

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            vec3 oc = centre - r.origin();
            auto a = r.direction().lengthSquared();
            auto h = dot(r.direction(), oc);
            auto c = oc.lengthSquared() - radius * radius;

            auto discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }

            auto sqtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range. 
            auto root = (h - sqtd) / a;
            if (!rayT.surrounds(root)) {
                root = (h + sqtd) / a;

                if (!rayT.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outwardNormal = (rec.p - centre) / radius;
            rec.setFaceNormal(r, outwardNormal);
            rec.mat = mat;

            return true;
        }

};

#endif