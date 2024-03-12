#include <cmath>

#include "Sphere.h"
#include "utils.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const Point3 center = center_at_time(r.time());
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
    rec.material = material;
    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}

Point3 Sphere::center_at_time(double time) const {
    return center + time * move_vec;
}

void Sphere::get_sphere_uv(const Point3& point, double& u, double& v) {
    double theta = std::acos(-point.Y());
    double phi = std::atan2(-point.Z(), point.X()) + pi;

    u = phi/ (2 * pi);
    v = theta / pi;
}