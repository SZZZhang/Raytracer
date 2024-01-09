#ifndef VECTOR_H
#define VECTOR_H

#include <ostream>

class Vec3 {

    public:
    inline Vec3() {};
    Vec3(double x, double y, double z);

    double X() const { return e[0]; };
    double Y() const { return e[1]; };
    double Z() const { return e[2]; };

    Vec3& operator+=(Vec3& vec);
    Vec3& operator-=(Vec3& vec);
    Vec3& operator*=(Vec3& vec);
    Vec3& operator/=(Vec3& vec);
    Vec3& operator-();

    double length() const;
    double length_squared() const;

    private:
    double e[3];
};

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.X() << ' ' << v.Y() << ' ' << v.Z();
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z());
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.X(), t*v.Y(), t*v.Z());
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.X() * v.X()
         + u.Y() * v.Y()
         + u.Z() * v.Z();
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.Y() * v.Z() - u.Z() * v.Y(),
                u.Z() * v.X() - u.X() * v.Z(),
                u.X() * v.Y() - u.Y() * v.X());
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}


using Point3 = Vec3;  
using Color = Vec3;   

#endif