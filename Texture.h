#pragma once

#include <memory>
#include <iostream>

#include "utils.h"
#include "Vec3.h"

class Texture {

    public: 
        virtual ~Texture() = default;

        virtual Color value(double u, double v, const Point3& point) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(Color c) : the_color{c} {}

        Color value(double u, double v, const Point3& point) const override {
            return the_color;
        }

    private: 
        Color the_color;
};



class CheckerTexture : public Texture {
    public:
        CheckerTexture(double scal, Color a, Color b)
            : scale{scal}, even{std::make_shared<SolidColor>(a)}, odd{std::make_shared<SolidColor>(b)} {}

        Color value(double u, double v, const Point3& point) const override {
            int u_int = static_cast<int>(std::floor((1.0 / scale) * u));
            int v_int = static_cast<int>(std::floor((1.0 / scale) * v));

            return ((u_int + v_int) % 2 == 0) ? even->value(u, v, point) : odd->value(u, v, point); 
        }

    private:
        double scale;
        std::shared_ptr<Texture> even;
        std::shared_ptr<Texture> odd;
};
