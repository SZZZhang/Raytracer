#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {

    public:
        HittableList() {}
        HittableList(std::shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<Hittable> object) { 
            objects.push_back(object); 
            box = Aabb(box, object->bounding_box());
        }
        void add(std::shared_ptr<HittableList> list) {
            for (auto object: list->objects) {
                add(object);
            }
        }
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

        std::vector<std::shared_ptr<Hittable>> objects;
        virtual Aabb bounding_box() const override { return box; }

    private:
        Aabb box;
};

inline bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
