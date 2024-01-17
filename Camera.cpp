#include "Camera.h"
#include "Hittable.h"
#include "Color.h"
#include "Material.h"
#include "utils.h"

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

Point3 Camera::rand_point_in_square(Point3 pixel_center) const {
    return pixel_center + (-0.5 + random_double(0.0,1.0)) * (horizontal/image_width)
        + (-0.5 + random_double(0.0,1.0)) * (vertical/image_height);
}

Ray Camera::get_ray(double u, double v) const {
    Point3 pixel_center = lower_left_corner + u*horizontal + v*vertical;
    return Ray(origin, rand_point_in_square(pixel_center));
}

Color get_color(const Ray& r, const Hittable& world, int max_depth) {
    HitRecord record;
    if (max_depth <= 0) {
        return Color(0.0,0.0,0.0);
    }
    if (world.hit(r, 0.001, infinity, record)) {
        Ray scattered;
        Color attenuation;
        if (record.material->scatter(r, record, attenuation, scattered)) {
            return attenuation * get_color(scattered, world, max_depth - 1);
        }
        return Color(0.0,0.0,0.0);
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
            double u = static_cast<double>(i) / static_cast<double>(image_width-1);
            double v = static_cast<double>(j) / static_cast<double>(image_height-1);
            Color pixel_color(0,0,0);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                Ray r =  get_ray(u, v);
                Color c = get_color(r, world, max_get_color_depth);
                pixel_color += c;
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n";
}
