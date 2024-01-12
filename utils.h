#pragma once

#include <cmath>
#include <limits>
#include <random>

#include "Vec3.h"

// Constants

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
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vec3 random_vec_unit() {
    Vec3 rand_point = Vec3(0,0,0);
    while(rand_point.length() == 0) {
        rand_point = Vec3(random_double(-1.0,1.0), random_double(-1.0,1.0), random_double(-1.0,1.0));
    }
    return unit_vector(rand_point);
}

// Common Headers
#include "Ray.h"
#include "Vec3.h"