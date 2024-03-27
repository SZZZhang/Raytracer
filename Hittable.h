#pragma once

#include "Ray.h"
#include "Aabb.h"

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    double u;
    double v;
    bool front_face;
    std::shared_ptr<Material> material;

    inline void set_face_normal(const Ray& r, Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& re) const = 0;
        virtual Aabb bounding_box() const = 0;
        virtual double pdf_value(const Point3& _p, const Vec3& _v) const { return 0.0; }
        virtual Vec3 rand_dir(const Vec3& _) const { return Vec3(1, 0, 0); }
};
