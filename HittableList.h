#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {

    public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    private:
    std::vector<std::shared_ptr<Hittable>> objects;
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool HittableList::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
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
