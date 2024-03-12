#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, std::shared_ptr<Material> mat, Vec3 mv_vec = Vec3(0,0,0))
         : center{cen}, radius{r}, material{mat}, move_vec{mv_vec} {};
        
        Point3 center_at_time(double time) const;
        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
        
        static void get_sphere_uv(const Point3& p, double& u, double& v);

    private:
        Point3 center;
        double radius;
        std::shared_ptr<Material> material;
        Vec3 move_vec;
};