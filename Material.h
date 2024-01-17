#pragma once

#include "utils.h"
#include "Hittable.h"

class Material {
    public:
        virtual ~Material() = default;
        virtual bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Color& a) : albedo(a) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            Vec3 scatter_direction = record.normal + random_vec_unit();

            if (scatter_direction.near_zero()) {
                scatter_direction = record.normal;
            }
            scattered_ray = Ray(record.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
};

class Metal : public Material {
    public:
        Metal(const Color& a) : albedo(a) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            Vec3 reflected = reflect(unit_vector(ray.direction()), record.normal);
            reflected = reflected + random_vec_unit();
            scattered_ray = Ray(record.p, reflected);
            attenuation = albedo;
            return dot(scattered_ray.direction(), record.normal) > 0;
        }

    private:
        Color albedo;
};
