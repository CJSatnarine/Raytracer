#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "rayTracer.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constantMedium : public hittable {
    public:
        constantMedium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> tex): boundary(boundary), negInvDensity(-1 / density), phaseFunction(make_shared<isotropic>(tex)) {}

        constantMedium(shared_ptr<hittable> boundary, double density, const colour& albedo) : boundary(boundary), negInvDensity(-1 / density), phaseFunction(make_shared<isotropic>(albedo)) {}

        bool hit(const ray& r, interval ray_t, hitRecord& rec) const override {
            // Print occasional samples when debugging. To enable, set enableDebug true.
            const bool enableDebug = false;
            const bool debugging = enableDebug && randomDouble() < 0.00001;

            hitRecord rec1, rec2;

            if (!boundary->hit(r, interval::universe, rec1))
                return false;

            if (!boundary->hit(r, interval(rec1.t+0.0001, infinity), rec2))
                return false;

            if (debugging) std::clog << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

            if (rec1.t < ray_t.min) rec1.t = ray_t.min;
            if (rec2.t > ray_t.max) rec2.t = ray_t.max;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            auto rayLength = r.direction().length();
            auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
            auto hitDistance = negInvDensity * log(randomDouble());

            if (hitDistance > distanceInsideBoundary)
                return false;

            rec.t = rec1.t + hitDistance / rayLength;
            rec.p = r.at(rec.t);

            if (debugging) {
                std::clog << "hit_distance = " <<  hitDistance << '\n'
                          << "rec.t = " <<  rec.t << '\n'
                          << "rec.p = " <<  rec.p << '\n';
            }

            rec.normal = vec3(1,0,0);   // arbitrary
            rec.frontFace = true;       // also arbitrary
            rec.mat = phaseFunction;

            return true;
        }

        aabb boundingBox() const override { 
            return boundary->boundingBox(); 
        }

    private:
        shared_ptr<hittable> boundary;
        double negInvDensity;
        shared_ptr<material> phaseFunction;
    };

#endif