#pragma once

#include <algorithm>
#include <vector>

#include "Hittable.h"

class BhvNode : public Hittable {
    public:

    BhvNode();

    BhvNode(std::vector<std::shared_ptr<Hittable>> objects) {
        if (objects.size() == 1) {
            box = objects[0]->bounding_box();
            left = objects[0];
            return;
        }
        
        std::vector<double> min_bound = {infinity, infinity, infinity};
        std::vector<double> max_bound = {neg_infinity, neg_infinity, neg_infinity};

        for (auto object : objects) {
            for (int a  = 0; a < 3; ++a) {
                min_bound[a] = std::fmin(min_bound[a], object->bounding_box().min[a]);
                max_bound[a] = std::fmax(max_bound[a], object->bounding_box().max[a]);
            }
        }

        int axis = 0;
        double max_size = 0;
        for (int a = 0; a < 3; ++a) {
            if (max_size < max_bound[a] - min_bound[a]) {
                max_size = max_bound[a] - min_bound[a];
                axis = a;
            }
        }
       
        std::sort(objects.begin(), objects.end(), 
            [axis] (std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b) {
                return a->bounding_box().min[axis] < b->bounding_box().min[axis];
        });

        if (objects.size() == 2) {
            left = objects[0];
            right = objects[1];
            box = Aabb(left->bounding_box(), right->bounding_box());
        } else {
            std::size_t half_size = objects.size() / 2;
            left = std::make_shared<BhvNode>(std::vector<std::shared_ptr<Hittable>>(objects.begin(), objects.begin() + half_size));
            right = std::make_shared<BhvNode>(std::vector<std::shared_ptr<Hittable>>(objects.begin() + half_size, objects.end()));
            box = Aabb(left->bounding_box(), right->bounding_box());
        }
    }

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& re) const override {
        if (!box.hit(r, Interval(t_min, t_max))) {
            return false;
        }
        
        bool left_hit = left ? left->hit(r, t_min, t_max, re) : false;
        bool right_hit = right ? right->hit(r, t_min, (left_hit ? re.t : t_max) , re) : false;

        return left_hit || right_hit;
    }

    Aabb bounding_box() const override {
        return box;
    }

    private:
        std::shared_ptr<Hittable> left;
        std::shared_ptr<Hittable> right;
        Aabb box;
};