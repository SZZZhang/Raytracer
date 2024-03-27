#pragma once

#include "Hittable.h"
#include "utils.h"

class Camera {
    public:
        void initialize(double aspect_ratio, int image_width);
        void render(Hittable& world, Hittable& lights) const;
        Point3 rand_point_in_square(Point3 pixel_center) const;

        double aspect_ratio;
        int image_width;
        

        int samples_per_pixel = 10;
        int max_get_color_depth = 50;

        Point3 lookfrom;
        //Point3 lookfrom = Point3(278, 278, -800);
        Point3 lookat;
        //Point3 lookat = Point3(278, 278, 0);
        Vec3 vertical_up = Point3(0.0, 1.0, 0.0);

        double focus_dist = 3.4; 
        double defocus_angle = 0;

        double vertical_fov_rad = 0.7;
    private:

        // Cornellbox:

        //const Point3 lookfrom = Point3(-2.0, 2.0, 1.0);
        //const Point3 lookfrom = Point3(0.0, 0.0, 9.0);
        //const Point3 lookat = Point3(0.0, 0.0, 0.0);

        Vec3 w, u, v; // Orthonormal basis for camera orientation

        int image_height;

        double viewport_height;
        double viewport_width;

        Vec3 horizontal;
        Vec3 vertical;
        Point3 upper_left_corner;

        int stratified_samples_per_pixel_width;
        double stratified_sample_width;

        double defocus_radius;
        Vec3 defocus_horizontal;
        Vec3 defocus_vertical;

        Ray get_ray(double u, double v) const;
};

