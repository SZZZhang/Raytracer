#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.X()) << ' '
        << static_cast<int>(255.999 * pixel_color.Y()) << ' '
        << static_cast<int>(255.999 * pixel_color.Z()) << '\n';
}

#endif