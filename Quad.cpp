#include "Quad.h"

#include <iostream>

bool Quad::hit(const Ray& r, double t_min, double t_max, HitRecord& re) const {
    // ray and norm are perpendicular
    if (std::fabs(dot(norm, r.direction())) < 1e-8) {
        return false;
    }

    double t = (D - dot(norm, r.origin())) / (dot(norm, r.direction()));

    if (t < t_min || t > t_max) {
        return false;
    }

    Point3 poi = r.at(t);
    Vec3 dist = poi - q;
    Vec3 w = norm_scaled / (dot(norm_scaled, norm_scaled));
    double alpha = dot(w, cross(dist, v));
    double beta = dot(w, cross(u, dist));

    // on the same plane but not in the quad
    if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) {
        //std::cerr << alpha << "  " << beta << std::endl;
        return false;
    }

    re.t = t;
    re.p = poi;
    re.normal = norm;
    re.material = material;
    re.u = alpha;
    re.v = beta;
    Vec3 face_norm = Vec3(norm);
    re.set_face_normal(r, face_norm);

    return true;
}

double Quad::pdf_value(const Point3& origin, const Vec3& dir) const {
    HitRecord record;
    if (!this->hit(Ray(origin, dir), 0.001, infinity, record)) {
        return 0;
    }

    double distance_squared = record.t * record.t * dir.length_squared();
    double cosine = std::fabs(dot(dir, record.normal)/dir.length());

    return distance_squared / (cosine * area);
}

Vec3 Quad::rand_dir(const Point3& origin) const {
    Vec3 rand_point = q + (random_double(0.0,1.0) * u) + (random_double(0.0,1.0) * v);
    return rand_point - origin;
}

// rotate theta around y axis
Vec3 rotate_y(Vec3 v, double theta) {
    return Vec3(v.X() * cos(theta) + v.Z() * sin(theta), 
                v.Y(),
                -1 * v.X() * sin(theta) + v.Z() * cos(theta));
}

std::shared_ptr<HittableList> Quad::get_box(const Point3& q, const Vec3& u_, const Vec3& v_, double h, 
    std::shared_ptr<Material> mat, double theta_deg) {
    
    std::shared_ptr<HittableList> sides = std::make_shared<HittableList>();

    Vec3 height = Vec3(0,h,0);

    double theta = degrees_to_radians(theta_deg);
    Vec3 u = rotate_y(u_, theta);
    Vec3 v = rotate_y(v_, theta);

    sides->add(std::make_shared<Quad>(q,  u,  v, mat)); // base
    sides->add(std::make_shared<Quad>(q + height,  u, v, mat)); // top
    sides->add(std::make_shared<Quad>(q, u, height, mat)); // front
    sides->add(std::make_shared<Quad>(q + v, u,  height, mat)); // back
    sides->add(std::make_shared<Quad>(q + u, v,  height, mat)); // right
    sides->add(std::make_shared<Quad>(q, v, height, mat)); // left

    return sides;
}
