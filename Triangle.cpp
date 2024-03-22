#include "Triangle.h"

#include <iostream>

bool Triangle::hit(const Ray& r, double t_min, double t_max, HitRecord& re) const {
    double D = dot(norm, q);
    Point3 x = Point3(0,0,-100);

    // ray and norm are perpendicular
    if (std::fabs(dot(norm, r.direction())) < 1e-8) {
        HittableList::case1++;
        return false;
    }

    double t = (D - dot(norm, r.origin())) / (dot(norm, r.direction()));

    if (t < t_min || t > t_max) {
        HittableList::case2++;
        return false;
    }

    Vec3 norm_scaled = cross(u,v);

    Point3 poi = r.at(t);
    Vec3 dist = poi - q;
    Vec3 w = norm_scaled / (dot(norm_scaled, norm_scaled));
    double alpha = dot(w, cross(dist, v));
    double beta = dot(w, cross(u, dist));

    // on the same plane but not in the quad
    if (!(alpha >= 0 && beta >= 0 && alpha + beta <= 1)) {
        //std::cerr << alpha << "  " << beta << std::endl;
        HittableList::case3++;
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
    HittableList::case4++;

    return true;
}
