#include "Camera.h"
#include "Hittable.h"
#include "Color.h"
#include "Material.h"
#include "utils.h"
#include "Pdf.h"

#include <iostream>

void Camera::initialize(double aspect_ratio_, int image_width_) {
    aspect_ratio = aspect_ratio_;
    image_width = image_width_;
    image_height = static_cast<int>(image_width / aspect_ratio);
    
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vertical_up, w));
    v = cross(w, u);

    viewport_height = 2.0 * tan(vertical_fov_rad/2) * focus_dist;
    viewport_width = aspect_ratio * viewport_height;

    horizontal = viewport_width * u;
    vertical = -1 * viewport_height * v;
    upper_left_corner = lookfrom - (focus_dist * w) - horizontal/2 - vertical/2;

    stratified_samples_per_pixel_width = std::sqrt(samples_per_pixel);
    stratified_sample_width = ((horizontal).length()/(image_width * stratified_samples_per_pixel_width));

    defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));
    defocus_horizontal = horizontal * defocus_radius;
    defocus_vertical = vertical * defocus_radius;

}

Point3 Camera::rand_point_in_square(Point3 pixel_center) const {
    return pixel_center + (random_double(0.0, stratified_sample_width)) * (horizontal/image_width)
        + (random_double(0.0,stratified_sample_width)) * (vertical/image_height);
}

Ray Camera::get_ray(double u, double v) const {
    Point3 pixel_upper_left_corner = upper_left_corner + u*horizontal + v*vertical;
    Vec3 random_vec_in_disk = random_in_unit_disk();
    Point3 ray_origin = defocus_radius == 0 ? lookfrom : 
        lookfrom + random_vec_in_disk.X() * defocus_horizontal + random_vec_in_disk.Y() * defocus_vertical;
    
    double ray_time = random_double(0.0, 1.0);
    //return Ray(ray_origin, pixel_center - ray_origin, ray_time); // No antialiasing
    return Ray(ray_origin, rand_point_in_square(pixel_upper_left_corner - ray_origin), ray_time);
}

int case1 = 0, case2 = 0, case3 = 0, case4 =0;

Color get_color(const Ray& r, const Hittable& world, int max_depth) {
    HitRecord record;
    if (max_depth <= 0) {
        ++case1;
        return Color(0.0,0.0,0.0);
    }
    if (world.hit(r, 0.00001, infinity, record)) {
        Ray scattered;
        Color attenuation;
        Color emitted_color = record.material->emitted(record.u, record.v, record.p);

        if (!record.material->scatter(r, record, attenuation, scattered)) {
        ++case2;
            return emitted_color;
        }

        ++case3;
        // CosinePdf surface_pdf(record.normal);
        // scattered = Ray(record.p, surface_pdf.generate(), r.time());
        // double pdf_val = surface_pdf.value(scattered.direction());

        // Color scattered_col = (attenuation * get_color(scattered, world, max_depth - 1))/pdf_val;
        // return scattered_col;
        Color scattered_col = get_color(scattered, world, max_depth - 1);
        return attenuation * scattered_col + emitted_color;
    } else {
        // Background
        ++case4;
        return Color(0,0,0);
    
        // Beige: 
        //return Color(207.0/250.0, 185.0/250.0, 151.0/250.0);

        // Blueish white background
        Vec3 unit_direction = unit_vector(r.direction()); 
        auto t = 0.5*(unit_direction.Y() + 1.0);
        return Color((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
    }
}

void Camera::render(Hittable& world) const {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::cerr << "\rScanlines remaining: " << image_height - 1 - j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            
            Color pixel_color(0,0,0);
            // stratification;
            for (int y = 0; y < stratified_samples_per_pixel_width; ++y) {
                for (int x = 0; x < stratified_samples_per_pixel_width; ++x) {
                    double u = (static_cast<double>(i) + static_cast<double>(x) * stratified_sample_width)
                        / static_cast<double>(image_width-1);
                    double v = (static_cast<double>(j) + static_cast<double>(y) * stratified_sample_width)
                        / static_cast<double>(image_height-1);
    
                    Ray r = get_ray(u, v);
                    Color c = get_color(r, world, max_get_color_depth);
                    pixel_color += c;
                }
            }

            write_color(std::cout, pixel_color, stratified_samples_per_pixel_width * stratified_samples_per_pixel_width);
        }
    }

    // double u = static_cast<double>(image_width / 2) / static_cast<double>(image_width-1);
    // double v = static_cast<double>(image_height / 2) / static_cast<double>(image_height-1);
    // get_color(get_ray(u, v), world, max_get_color_depth);

    std::cerr << "\nDone\n";

    std::cerr << "max depth: " << case1 << ", light: " << case2 << ", bounce: " << case3 << ", background: " << case4 << std::endl;
}
