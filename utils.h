#pragma once

#include <cmath>
#include <limits>
#include <random>

#include "Vec3.h"

// Constants

// TODO: remove
#include <iostream>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double random_double(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vec3 random_vec_unit() {
    Vec3 rand_point = Vec3(0,0,0);
    while(rand_point.length() == 0 || rand_point.length() > 1) {
        rand_point = Vec3(random_double(-1.0,1.0), random_double(-1.0,1.0), random_double(-1.0,1.0));
    }
    return unit_vector(rand_point);
}

inline Vec3 random_in_unit_disk() {
    Vec3 rand_point = Vec3(0,0,0);
    while(rand_point.length() == 0 || rand_point.length() > 1) {
        rand_point = Vec3(random_double(-1.0,1.0), random_double(-1.0,1.0), 0);
    }
    return unit_vector(rand_point);
}

inline Vec3 reflect(const Vec3& v, const Vec3& normal) {
    return v - 2 * dot(v, normal) * normal;
}

inline Vec3 refract(const Vec3& ray, const Vec3& normal, double refract_ratio) {
    double cos_theta = fmin(dot(-unit_vector(ray), normal), 1.0);
    Vec3 refracted_perpendicular = refract_ratio * (ray + (cos_theta * normal));
    Vec3 refracted_parallel = -1 * sqrt(fabs(1.0 - refracted_perpendicular.length_squared())) * normal;
    return unit_vector(refracted_perpendicular + refracted_parallel);
}

// Common Headers
#include "Ray.h"
#include "Vec3.h"