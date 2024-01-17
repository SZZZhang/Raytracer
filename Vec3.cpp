#include <cmath>

#include "Vec3.h"

Vec3::Vec3(double x, double y, double z) : e{ x, y, z} {}

Vec3& Vec3::operator+=(Vec3& vec) {
    e[0] += vec.X();
    e[1] += vec.Y();
    e[2] += vec.Z();
    return *this;
}

Vec3& Vec3::operator-=(Vec3& vec) {
    e[0] -= vec.X();
    e[1] -= vec.Y();
    e[2] -= vec.Z();
    return *this;
}

Vec3& Vec3::operator*=(Vec3& vec) {
    e[0] *= vec.X();
    e[1] *= vec.Y();
    e[2] *= vec.Z();
    return *this;
}

Vec3& Vec3::operator/=(Vec3& vec) {
    e[0] /= vec.X();
    e[1] /= vec.Y();
    e[2] /= vec.Z();
    return *this;
}

Vec3& Vec3::operator-() {
    e[0] *= -1;
    e[1] *= -1;
    e[2] *= -1;
    return *this;
}

double Vec3::length() const {
    return std::sqrt(length_squared());
}

double Vec3::length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool Vec3::near_zero() const {
    double tiny = 1e-8;
    return (std::fabs(X()) < tiny && std::fabs(Y()) < tiny && std::fabs(Z()) < tiny);
}
