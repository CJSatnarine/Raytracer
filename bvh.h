#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include <algorithm>
#include "hittable.h"
#include "hittableList.h"
#include "rayTracer.h"

class bvh_node : public hittable {
  public:
    bvh_node(hittableList list) : bvh_node(list.objects, 0, list.objects.size()) {
        // This constructor (without span indices) creates an implicit copy of the hittable list, which we will modify. The lifetime of the copied list only extends until this constructor exits. 
    }
    
    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
        int axis = randomInt(0,2);

        auto comparator = (axis == 0) ? boxXCompare
                        : (axis == 1) ? boxYCompare
                                      : boxZCompare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            left = objects[start];
            right = objects[start+1];
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }

        bBox = aabb(left->boundingBox(), right->boundingBox());
    }

    bool hit(const ray& r, interval ray_t, hitRecord& rec) const override {
        if (!bBox.hit(r, ray_t)) return false;

        bool hitLeft = left->hit(r, ray_t, rec);
        bool hitRight = right->hit(r, interval(ray_t.min, hitLeft ? rec.t : ray_t.max), rec);

        return hitLeft || hitRight;
    }

    aabb boundingBox() const override { return bBox; }

  private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bBox;

    static bool boxCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axisIndex) {
        auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
        auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
        return aAxisInterval.min < bAxisInterval.min;
    }

    static bool boxXCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return boxCompare(a, b, 0);
    }

    static bool boxYCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return boxCompare(a, b, 1);
    }

    static bool boxZCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return boxCompare(a, b, 2);
    }
};

#endif