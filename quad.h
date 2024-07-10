#ifndef QUAD_H
#define QUAD_H

#include "rayTracer.h"
#include "hittable.h"

class quad : public hittable {
    public: 
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> material) : Q(Q), u(u), v(v), mat(material) {
            auto n = cross(u, v);
            normal = unitVector(n);
            D = dot(normal, Q);
            w = n / dot(n, n);
            
            setBoundingBox();
        }

        virtual void setBoundingBox() {
            // Computer the bounding box of all four vertices.
            auto boundingBoxDiagonal1 = aabb(Q, Q + u + v);
            auto boundingBoxDiagonal2 = aabb(Q + u, Q + v);
            bBox = aabb(boundingBoxDiagonal1, boundingBoxDiagonal2);
        }   

        aabb boundingBox() const override {
            return bBox;
        }

        bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
            auto denom = dot(normal, r.direction());

            // No hit if the ray is parallel to the plane. 
            if (fabs(denom) < 1e-8) return false;

            // Return false if the hit point parameter t is outside of the ray interval. 
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!rayT.contains(t)) return false;

            // Determine the hit point lies within the planar shape using its plane coordinates. 
            auto intersection = r.at(t);
            vec3 planarHitpointVector = intersection - Q;
            auto alpha = dot(w, cross(planarHitpointVector, v));
            auto beta = dot(w, cross(u, planarHitpointVector));

            if (!isInterior(alpha, beta, rec)) return false;

            // Ray hits the 2D shape. Set the rest of the hit record and return true;
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.setFaceNormal(r, normal);

            return true;
        }

        virtual bool isInterior(double a, double b, hitRecord& rec) const {
            interval unitInterval = interval(0, 1);

            // Given the hit point in plane coordinates, return false if it is outside the primitive, otherwise set the hit record UV coordinates and return true. 
            if (!unitInterval.contains(a) || !unitInterval.contains(b)) {
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        point3 Q;
        vec3 u;
        vec3 v;
        vec3 w;
        shared_ptr<material> mat;
        aabb bBox;
        vec3 normal;
        double D;
};


#endif