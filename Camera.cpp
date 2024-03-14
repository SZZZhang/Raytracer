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
    w{unit_vector(lookfrom - lookat)},
    u{unit_vector(cross(vertical_up, w))},
    v{cross(w, u)},
   // vertical_fov_rad{1.5708}, // 90 degrees
    vertical_fov_rad{0.7}, // 40 degrees
    //vertical_fov_rad{0.349}, // 20 degrees
    viewport_height{2.0 * tan(vertical_fov_rad/2) * focus_dist},
    viewport_width{aspect_ratio * viewport_height},
    horizontal{viewport_width * u},
    vertical{-1 * viewport_height * v},
    upper_left_corner{lookfrom - (focus_dist * w) - horizontal/2 - vertical/2},
    defocus_radius{focus_dist * tan(degrees_to_radians(defocus_angle/2))},
    defocus_horizontal{horizontal * defocus_radius},
    defocus_vertical{vertical * defocus_radius}
      {}

Point3 Camera::rand_point_in_square(Point3 pixel_center) const {
    return pixel_center + (-0.5 + random_double(0.0,1.0)) * (horizontal/image_width)
        + (-0.5 + random_double(0.0,1.0)) * (vertical/image_height);
}

Ray Camera::get_ray(double u, double v) const {
    Point3 pixel_center = upper_left_corner + u*horizontal + v*vertical;
    Vec3 random_vec_in_disk = random_in_unit_disk();
    Point3 ray_origin = defocus_radius == 0 ? lookfrom : 
        lookfrom + random_vec_in_disk.X() * defocus_horizontal + random_vec_in_disk.Y() * defocus_vertical;
    
    double ray_time = random_double(0.0, 1.0);
    
    //return Ray(ray_origin, pixel_center - ray_origin, ray_time); // No antialiasing
    return Ray(ray_origin, rand_point_in_square(pixel_center - ray_origin), ray_time);
}

Color get_color(const Ray& r, const Hittable& world, int max_depth) {
    HitRecord record;
    if (max_depth <= 0) {
        return Color(0.0,0.0,0.0);
    }
    if (world.hit(r, 0.00001, infinity, record)) {
        Ray scattered;
        Color attenuation;
        Color emitted_color = record.material->emitted(record.u, record.v, record.p);

        if (!record.material->scatter(r, record, attenuation, scattered)) {
            return emitted_color;
        }

        return attenuation * get_color(scattered, world, max_depth - 1);
    } else {
        // Background
        return Color(0,0,0);

        // Blueish white background
        // Vec3 unit_direction = unit_vector(r.direction()); 
        // auto t = 0.5*(unit_direction.Y() + 1.0);
        // return Color((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
    }
}

void Camera::render(Hittable& world) const {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::cerr << "\rScanlines remaining: " << image_height - 1 - j << ' ' << std::flush;
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

    // double u = static_cast<double>(image_width / 2) / static_cast<double>(image_width-1);
    // double v = static_cast<double>(image_height / 2) / static_cast<double>(image_height-1);
    // get_color(get_ray(u, v), world, max_get_color_depth);

    std::cerr << "\nDone\n";
}
