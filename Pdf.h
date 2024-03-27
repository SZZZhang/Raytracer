#pragma once

#include "utils.h"
#include "Hittable.h"

class Pdf {
    public:
        virtual ~Pdf() {};
        virtual double value(const Vec3& direction) const = 0;
        virtual Vec3 generate() const = 0;
};

struct Basis {
    Vec3 u() const { return axis[0]; }
    Vec3 v() const { return axis[1]; }
    Vec3 w() const { return axis[2]; }

    void build_from_w(const Vec3& w) {
        Vec3 unit_w = unit_vector(w);
        Vec3 a = (fabs(unit_w.X()) > 0.9) ? Vec3(0,1,0) : Vec3(1,0,0);
        Vec3 v = unit_vector(cross(unit_w, a));
        Vec3 u = cross(unit_w, v);
        axis[0] = u;
        axis[1] = v;
        axis[2] = unit_w;
    }

    Vec3 local(const Vec3& a) const {
        return a.X() * axis[0] + a.Y() * axis[1] + a.Z() * axis[2];
    }

    Vec3 axis[3];
};

inline Vec3 random_cosine_direction() {
    auto r1 = random_double(0.0, 1.0);
    auto r2 = random_double(0.0, 1.0);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);
    auto z = sqrt(1-r2);

    return Vec3(x, y, z);
}

class CosinePdf : public Pdf {
    public:
        CosinePdf(const Vec3& w) { uvw.build_from_w(w); }

        double value(const Vec3& direction) const override {
            double cosine_theta = dot(unit_vector(direction), uvw.w());
            return std::fmax(0, cosine_theta/pi);
        }

        Vec3 generate() const override {
            return uvw.local(random_cosine_direction());
        }

    private:
        Basis uvw;
};

class HittablePdf : public Pdf {
    public:
        HittablePdf(const Hittable& object, const Point3& origin)
        : object{object}, origin{origin} {}

        double value(const Vec3& direction) const override {
            return object.pdf_value(origin, direction);
        }

        Vec3 generate() const override {
            return object.rand_dir(origin);
        }

    private:
        const Hittable& object;
        Point3 origin;
};

class MixturePdf : public Pdf {
    public: 
        MixturePdf(std::shared_ptr<Pdf> p1, std::shared_ptr<Pdf> p2) : p1{p1}, p2{p2} {}

        double value(const Vec3& direction) const override {
            return 0.25 * p1->value(direction) + 0.75 * p2->value(direction);
        }

        Vec3 generate() const override {
            if (random_double(0.0,1.0) < 0.25) {
                return p1->generate();
            } 
            return p2->generate();
        }

    private:
        std::shared_ptr<Pdf> p1;
        std::shared_ptr<Pdf> p2;
};
