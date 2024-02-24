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

class Dielectric : public Material {
    public:
        Dielectric(const Color& a, double refract_index) : 
            albedo{a}, refract_index{refract_index} {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            double refract_ratio = record.front_face ? 1.0/refract_index : refract_index;
            attenuation = Color(1.0,1.0,1.0);
            
            double cos_theta = fmin(dot(-unit_vector(ray.direction()), record.normal), 1.0); 
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
            double rand_double = random_double(0.0, 1.0);

            // Total internal reflection
            if (refract_ratio * sin_theta > 1.0 || 
                reflectance(cos_theta, refract_index) > rand_double)
            {
                Vec3 reflected = reflect(unit_vector(ray.direction()), record.normal);
                scattered_ray = Ray(record.p, reflected);
            } else {
                Vec3 refracted = refract(unit_vector(ray.direction()), record.normal, refract_ratio);
                scattered_ray = Ray(record.p, refracted);
            }
            return true;
        }

    private:
        Color albedo;
        double refract_index;
    
    static double reflectance(double cosine, double refract_index) {
        // Schlick's approximation for reflectance
        double r0 = (1 - refract_index) / (1 + refract_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};
