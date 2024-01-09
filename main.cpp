#include <iostream>
#include <cmath>
#include <memory>

#include "Vec3.h"
#include "Ray.h"
#include "Color.h"
#include "Sphere.h"
#include "HittableList.h"
#include "utils.h"
#include "Camera.h"

color ray_color(const ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.Y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}
// Image
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 1;

// Camera
Camera camera; 

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
const Sphere sphere(sphere_center, radius);
const double t_min = 0;
const double t_max = 100;

color get_color(const ray& r, const Hittable& world) {
    hit_record record;
    if (world.hit(r, 0, infinity, record)) {
        return color((record.normal.X() + 1)/2, (record.normal.Y() + 1)/2, (record.normal.Z() + 1)/2);
    } else {
        Vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.Y() + 1.0);
        return color((1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0));
    }
}

// set ray eq = circle eq, this will be the poi 
// normal = ray (point = poi, direction = poi - center)

int main() {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // world
    HittableList world;
    world.add(std::make_shared<Sphere>(point3(0,0,-1), 0.5));
    world.add(std::make_shared<Sphere>(point3(0,-100.5,-1), 100));

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = (i + random_double()) / (image_width-1);
            auto v = (j + random_double()) / (image_height-1);
            ray r = camera.get_ray(u, v);
            color c = get_color(r, world);
            write_color(std::cout, c, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n";
}
