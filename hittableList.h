#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rayTracer.h"
#include <vector>

class hittableList : public hittable {
    public: 
        std::vector<shared_ptr<hittable>> objects;

        hittableList() {}
        hittableList(shared_ptr<hittable> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit (const ray& r, interval rayT, hitRecord& rec) const override {
            hitRecord temporaryRec;
            bool hitAnything = false;

            auto closestSoFar = rayT.max;

            for (const auto& object : objects) {
               if (object->hit(r, interval(rayT.min, closestSoFar), temporaryRec)) {
                    hitAnything = true;
                    closestSoFar = temporaryRec.t;
                    rec = temporaryRec;
                }
            }

            return hitAnything;
        }
};

#endif