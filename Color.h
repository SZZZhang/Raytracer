#pragma once

#include "Vec3.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <format>

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

void write_color_to_buff(Color pixel_color, int samples_per_pixel, unsigned char* buff, size_t& buff_idx) {
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

    buff[buff_idx] = static_cast<unsigned char>(256 * std::clamp(r, 0.0, 0.999));
    buff[buff_idx + 1] = static_cast<unsigned char>(256 * std::clamp(g, 0.0, 0.999));
    buff[buff_idx + 2] = static_cast<unsigned char>(256 * std::clamp(b, 0.0, 0.999));

    buff_idx += 3;
}

void write_colors(std::ostream &out, unsigned char* buff, int image_width, int image_height) {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    size_t buff_len = image_width * image_height * 3;
    for (size_t i = 0; i < buff_len; i += 3) {
        out << static_cast<int>(buff[i]) << ' '
            << static_cast<int>(buff[i + 1]) << ' '
            << static_cast<int>(buff[i + 2]) << '\n';
    }
}
