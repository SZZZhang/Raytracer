#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"

#include <iostream>
#include <algorithm>
#include <cmath>

inline double linear_to_gamma(double linear) {
    return std::sqrt(linear);
}

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.X();
    auto g = pixel_color.Y();
    auto b = pixel_color.Z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}
#endif