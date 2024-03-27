#pragma once

#include "utils.h"
#include "Hittable.h"
#include "Texture.h"
#include "Pdf.h"

class Material {
    public:
        virtual ~Material() = default;
        virtual bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const = 0;
        virtual Color emitted(double u, double v, const Point3& point) const { return Color(0, 0, 0); }
        virtual double scattering_pdf(const Ray& r_in, const HitRecord& record, const Ray& scattered) const { return 0; }
};

class Lambertian : public Material {
    public:
        Lambertian(const Color& a) : texture{std::make_shared<SolidColor>(a)} {}
        Lambertian(const std::shared_ptr<Texture>& text) : texture{text} {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            Basis uvw;
            uvw.build_from_w(record.normal);
            Vec3 scatter_direction = uvw.local(random_cosine_direction());

            if (scatter_direction.near_zero()) {
                scatter_direction = record.normal;
            }
            scattered_ray = Ray(record.p, scatter_direction, ray.time());
            attenuation = texture->value(record.u, record.v, record.p);
            return true;
        }
        double scattering_pdf(const Ray& r_in, const HitRecord& record, const Ray& scattered) const override {
            double cos_theta = dot(record.normal, unit_vector(scattered.direction()));
            return cos_theta < 0 ? 0 : cos_theta/pi;
        }

    private:
        std::shared_ptr<Texture> texture;
};

class Metal : public Material {
    public:
        Metal(const Color& a) : albedo(a) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            Vec3 reflected = reflect(unit_vector(ray.direction()), record.normal);
            reflected = reflected + random_vec_unit();
            scattered_ray = Ray(record.p, reflected, ray.time());
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
                scattered_ray = Ray(record.p, reflected, ray.time());
            } else {
                Vec3 refracted = refract(unit_vector(ray.direction()), record.normal, refract_ratio);
                scattered_ray = Ray(record.p, refracted, ray.time());
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

class DiffuseLight : public Material {
    public:
        DiffuseLight(Color color) : texture{ std::make_shared<SolidColor>(color) } {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override {
            attenuation = Color(0.1,0.1,0.1);
            return false;
        }

        Color emitted(double u, double v, const Point3& point) const override {
            return texture->value(u, v, point);
        };

    private:
        std::shared_ptr<Texture> texture;
};
