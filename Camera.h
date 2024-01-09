#pragma once

#include "Hittable.h"
#include "utils.h"

class Camera {
    public:
        double aspect_ratio;
        int image_width;
        
        Camera(double aspect_ratio, int image_width);
        void render(Hittable& world) const;

    private:
        const int samples_per_pixel = 1;

        // Image
        const int image_height;

        const double viewport_height;
        const double viewport_width;
        const double focal_length = 1.0;

        const Point3 origin;
        const Vec3 horizontal;
        const Vec3 vertical;
        const Point3 lower_left_corner;
        
        Ray get_ray(double u, double v) const;
};

