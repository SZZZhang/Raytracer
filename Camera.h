#pragma once

#include "Hittable.h"
#include "utils.h"

class Camera {
    public:
        double aspect_ratio;
        int image_width;
        
        Camera(double aspect_ratio, int image_width);
        void render(Hittable& world) const;
        Point3 rand_point_in_square(Point3 pixel_center) const;

    private:
        const int samples_per_pixel = 50;
        const int max_get_color_depth = 50;

        const Point3 lookfrom = Point3(-2.0, 2.0, 1.0);
        const Point3 lookat = Point3(0.0, 0.0, -1.0);
        const Vec3 vertical_up = Point3(0.0, 1.0, 0.0);

        const Vec3 w, u, v; // Orthonormal basis for camera orientation

        const int image_height;

        const double focal_length;
        const double vertical_fov_rad;

        const double viewport_height;
        const double viewport_width;

        const Vec3 horizontal;
        const Vec3 vertical;
        const Point3 upper_left_corner;
        
        Ray get_ray(double u, double v) const;
};

