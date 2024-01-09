#include "Camera.h"
#include "Hittable.h"
#include "Color.h"

#include <iostream>

Camera::Camera(double aspect_ratio, int image_width) :
    aspect_ratio{aspect_ratio},
    image_width{image_width},
    image_height{static_cast<int>(image_width / aspect_ratio)},
    viewport_height{2.0},
    viewport_width{aspect_ratio * viewport_height},
    focal_length{1.0},
    origin{Point3(0, 0, 0)},
    horizontal{Vec3(viewport_width, 0.0, 0.0)},
    vertical{Vec3(0.0, viewport_height, 0.0)},
    lower_left_corner{origin - horizontal/2 - vertical/2 - Point3(0, 0, focal_length)}
      {}

Ray Camera::get_ray(double u, double v) const {
    return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}

Color get_color(const Ray& r, const Hittable& world) {
    HitRecord record;
    if (world.hit(r, 0, infinity, record)) {
        return 0.5 * (record.normal + Color(1,1,1));
    } else {
        // Background
        Vec3 unit_direction = unit_vector(r.direction()); 
        auto t = 0.5*(unit_direction.Y() + 1.0);
        return Color((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
    }
}

void Camera::render(Hittable& world) const {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = (i + random_double()) / (image_width-1);
            auto v = (j + random_double()) / (image_height-1);
            Ray r =  get_ray(u, v);
            Color c = get_color(r, world);
            write_color(std::cout, c, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n";
}
