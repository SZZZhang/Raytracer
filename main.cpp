#include <cmath>
#include <memory>

#include "Vec3.h"
#include "Sphere.h"
#include "HittableList.h"
#include "utils.h"
#include "Camera.h"
#include "Material.h"

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
int main() {

    Camera camera = Camera(aspect_ratio, image_width); 

    HittableList world;

    std::shared_ptr<Lambertian> mat_diffuse_ptr = std::make_shared<Lambertian>(Color(0.9, 0.9, 0.5));
    std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    std::shared_ptr<Metal> mat_metal_ptr = std::make_shared<Metal>(Color(0.5, 0.9, 0.9));
    std::shared_ptr<Dielectric> mat_glass_ptr = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);

    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5, mat_diffuse_ptr));
    world.add(std::make_shared<Sphere>(Point3(1,0,-1), 0.5, mat_metal_ptr));
    world.add(std::make_shared<Sphere>(Point3(-1,0,-1), -0.4, mat_glass_ptr));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100, mat_diffuse_background_ptr));
    
    camera.render(world);
}
