#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {

    public:

        inline static int case1 = 0, case2 = 0, case3 = 0, case4 = 0;
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

        // TODO: FIX? this is so the pdf value and rand_dir of a light can be easily extracted
        virtual double pdf_value(const Point3& p, const Vec3& v) const override { 
            for (auto object: objects) {
                return object->pdf_value(p, v);
            }
            return 0.0;
        }

        virtual Vec3 rand_dir(const Vec3& origin) const override { 
            for (auto object: objects) {
                return object->rand_dir(origin);
            }
            return Vec3(1,0,0);
        }

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
