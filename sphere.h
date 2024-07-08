#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rayTracer.h"

class sphere : public hittable {
    private:
        point3 centre1;
        double radius;
        shared_ptr<material> mat;
        bool isMoving;
        vec3 centreVec;
        aabb bBox;

        point3 sphereCentre(double time) const {
            // Linearly interpolate from centre1 to centre2 accoedingf to time, where t=0 yields centre1 and t=1 yields centre2. 
            return centre1 + time * centreVec;
        }

    public: 
        // Stationary sphere. 
        sphere(const point3& centre, double radius, shared_ptr<material> mat) : centre1(centre), radius(fmax(0, radius)), mat(mat), isMoving(false) {
            auto rvec = vec3(radius, radius, radius);
            bBox = aabb(centre1 - rvec, centre1 + rvec);
        }

        // Moving sphere. 
        sphere(const point3& centre1, const point3& centre2, double radius, shared_ptr<material> mat) : centre1(centre1), radius(fmax(0, radius)), mat(mat), isMoving(true) {
            auto rvec = vec3(radius, radius, radius);
            aabb box1(centre1 - rvec, centre1 + rvec);
            aabb box2(centre2 - rvec, centre2 + rvec);
            bBox = aabb(box1, box2);
            
            centreVec = centre2 - centre1;
        }

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            point3 centre = isMoving ? sphereCentre(r.time()) : centre1;
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

        aabb boundingBox() const override { 
            return bBox; 
        }

};

#endif