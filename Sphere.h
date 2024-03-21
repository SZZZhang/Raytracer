#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, std::shared_ptr<Material> mat, Vec3 mv_vec = Vec3(0,0,0))
         : center{cen}, radius{r}, material{mat}, move_vec{mv_vec}, 
         b_box{Aabb(center - Vec3(r,r,r), center + Vec3(r,r,r))} {
            // For moving spheres, merge the bounding boxes. 
            if (!(move_vec == Vec3(0,0,0))) {
                Aabb aabb2 = Aabb((center + move_vec) - Vec3(r,r,r), (center + move_vec) + Vec3(r,r,r));
                b_box = Aabb(b_box, aabb2);
            }
         };
        
        Point3 center_at_time(double time) const;
        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

        Aabb bounding_box() const override { return b_box; }
        
        static void get_sphere_uv(const Point3& p, double& u, double& v);

    private:
        Point3 center;
        double radius;
        std::shared_ptr<Material> material;
        Vec3 move_vec;
        Aabb b_box;
};