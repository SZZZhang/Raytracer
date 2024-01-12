#include <cmath>

#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const auto dist = r.origin() - center;
    const double a = dot(r.direction(), r.direction());
    const double b = 2 * dot(r.direction(), dist);
    const double c = dot(dist, dist) - radius * radius;

    if (b * b - 4 * a * c < 0) {
        return false;
    }
    
    double t = (- b - std::sqrt(b * b - 4 * a * c)) / (2 * a);

    if (t <= t_min || t >= t_max) {
        t = (- b + std::sqrt(b * b - 4 * a * c)) / (2 * a);
        if (t <= t_min || t >= t_max) {
            return false;
        }
    }

    rec.t = t;
    rec.p = r.at(t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}