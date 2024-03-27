#pragma once

#include <cmath>

#include "utils.h"

class Interval {
    public:
        Interval(double a, double b) : 
            min{std::fmin(a,b)}, max{std::fmax(a,b)} {}

        Interval expand(double delta) {
            double padding = delta/2;
            return Interval(min - padding, max + padding);
        };

        const double min;
        const double max;

};

class Aabb {
    public:
        Aabb() = default;
        Aabb(const Point3& a, const Point3& b) : 
            min{std::fmin(a.X(), b.X()), std::fmin(a.Y(), b.Y()), std::fmin(a.Z(), b.Z())},
            max{std::fmax(a.X(), b.X()), std::fmax(a.Y(), b.Y()), std::fmax(a.Z(), b.Z())} {}

        Aabb(const Aabb& box1, const Aabb& box2) :
            min{std::fmin(box1.min[0], box2.min[0]), std::fmin(box1.min[1], box2.min[1]), std::fmin(box1.min[2], box2.min[2])}, 
            max{std::fmax(box1.max[0], box2.max[0]), std::fmax(box1.max[1], box2.max[1]), std::fmax(box1.max[2], box2.max[2])} {}

        bool hit(const Ray& r, Interval rayT) const {
            double A[3] = { r.origin().X(), r.origin().Y(), r.origin().Z() };
            double b[3] = { r.direction().X(), r.direction().Y(), r.direction().Z() };

            for (int a = 0; a < 3; ++a) {
                double t0 = std::fmin((min[a] - A[a])/b[a], (max[a] - A[a])/b[a]);
                double t1 = std::fmax((min[a] - A[a])/b[a], (max[a] - A[a])/b[a]);

                double t_min = std::fmax(t0, rayT.min);
                double t_max = std::fmin(t1, rayT.max);
                if (t_max <= t_min) {
                    return false;
                }
            }
            return true;
        }

        void pad() {
            double delta = 1e-6;
            double padding = delta/2;
            for (int a = 0; a < 3; ++a) {
                if (max[a] - min[a] < delta) {
                    min[a] = min[a] - padding;
                    max[a] = max[a] + padding;
                }                
            }
        }

        double min[3];
        double max[3];
};