#include <iostream>
#include <cmath>
#include "Vec3.h"
#include "Ray.h"
#include "Color.h"

color ray_color(const ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.Y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}
// Image

const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);

// Render
const auto viewport_height = 2.0;
const auto viewport_width = aspect_ratio * viewport_height;
const auto focal_length = 1.0;

const auto origin = point3(0, 0, 0);
const auto horizontal = Vec3(viewport_width, 0, 0);
const auto vertical = Vec3(0, viewport_height, 0);
const auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

// sphere
const auto sphere_center = origin - Vec3(0, 0, focal_length);
const auto radius = 0.5;

bool hits_circle(const ray& r) {
    const auto dist = r.origin() - sphere_center;
    if (std::pow(2 * dot(r.direction(), dist), 2) - 4 * dot(r.direction(), r.direction())
        * (dot(dist, dist) - radius * radius) >= 0) {
        return true;
    }
    return false;
}

color get_color(const ray& r) {
    const auto dist = r.origin() - sphere_center;
    const double a = dot(r.direction(), r.direction());
    const double b = 2 * dot(r.direction(), dist);
    const double c = dot(dist, dist) - radius * radius;

    const double t = (- b + std::sqrt(b * b - 4 * a * c)) / (2 * a);

    const point3 poi = r.origin() + t * r.direction();

    Vec3 normal = poi - sphere_center;
    normal = unit_vector(normal);

    return Vec3((normal.X() + 1)/2, (normal.Y() + 1)/2, (normal.Z() + 1)/2);
}

// set ray eq = circle eq, this will be the poi 
// normal = ray (point = poi, direction = poi - center)

int main() {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color;
            if (hits_circle(r)) {
                pixel_color = get_color(r);
            } else {
                pixel_color = ray_color(r);
            }
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone\n";
}
