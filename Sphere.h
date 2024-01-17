#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, std::shared_ptr<Material> mat) : center{cen}, radius{r}, material{mat} {};

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Point3 center;
        double radius;
        std::shared_ptr<Material> material;
};