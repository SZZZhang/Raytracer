#pragma once

#include "utils.h"
#include "Vec3.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Aabb.h"

class Quad : public Hittable {
    public:
        Quad(const Point3& q_, const Vec3& u_, const Vec3& v_, std::shared_ptr<Material> mat)
            : q{q_}, u{u_}, v{v_}, norm{ unit_vector(cross(u, v)) }, material{mat}, 
            box{Aabb(q, q + u + v)} {
                box.pad();
                D = dot(norm, q);
                norm_scaled = cross(u,v);
                area = norm_scaled.length();
            }
        bool hit(const Ray& r, double t_min, double t_max, HitRecord& re) const override;
        Aabb bounding_box() const override { return box; }
        static std::shared_ptr<HittableList> get_box
            (const Point3& q, const Vec3& u, const Vec3& v, double h, std::shared_ptr<Material> mat, double theta = 0);
        double pdf_value(const Point3& origin, const Vec3& v) const override;
        Vec3 rand_dir(const Point3& origin) const override;

    private:
        Point3 q;
        Vec3 u;
        Vec3 v;
        Vec3 norm;
        Vec3 norm_scaled;
        double D;
        std::shared_ptr<Material> material;
        Aabb box;
        double area;
};
