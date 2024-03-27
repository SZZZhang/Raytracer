#include <cmath>
#include <memory>
#include <string>

#include "Vec3.h"
#include "Sphere.h"
#include "Quad.h"
#include "HittableList.h"
#include "utils.h"
#include "Camera.h"
#include "Material.h"
#include "BHV.h"
#include "Triangle.h"
#include "ObjLoader.h"

const double aspect_ratio = 1; //16.0 / 9.0;
const int image_width = 200;

void cornell_box(HittableList& world, HittableList& lights, Camera& cam) {

    std::shared_ptr<Lambertian> red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(std::make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.add(std::make_shared<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
    world.add(std::make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

    auto light_quad = std::make_shared<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light);
    lights.add(light_quad);
    world.add(light_quad);
    // Glass sphere
    //auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    //world.add(std::make_shared<Sphere>(Point3(190,90,190), 90, glass));
    
    //world.add(Quad::get_box(Point3(130, 0, 65), Point3(295, 165, 230), white));
    //world.add(Quad::get_box(Point3(265, 0, 295), Point3(430, 330, 460), white));
    world.add(Quad::get_box(Point3(130, 0, 65), Vec3(165, 0, 0), Vec3(0,0, 230), 165, white, -18));
    world.add(Quad::get_box(Point3(265, 0, 295), Vec3(165, 0, 0), Vec3(0, 0, 165), 330, white, 18));

    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);

    cam.samples_per_pixel = 64;// 4096;//1000;
    cam.max_get_color_depth = 200;
}

void single_sphere(HittableList& world, Camera& cam) {
    std::shared_ptr<Lambertian> red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    world.add(std::make_shared<Sphere>(Point3(190,90,190), 90, red));

    world.add(std::make_shared<Sphere>(Point3(20,90,190), 90, red));
}

void single_triangle(HittableList& world, Camera& cam) {
   
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    world.add(std::make_shared<Triangle>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
}

void obj(HittableList& world, HittableList& lights, Camera& cam) {
    //auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
    //std::shared_ptr<Lambertian> red  = std::make_shared<Lambertian>(Color(.65, .05, .05));

    ObjLoader::load(world, "./Skull_low_res.obj", white);

    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100, white));

    auto light_quad = std::make_shared<Quad>(Point3(-10, 10, -10), Vec3(10,0,0), Vec3(0,0,10), light);
    world.add(light_quad);
    lights.add(light_quad);


    cam.lookfrom = Point3(0, 0, -6);
    cam.lookat = Point3(0,0,0);

    cam.samples_per_pixel = 64;
}

void diamond_obj(HittableList& world, Camera& cam) {
    auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 2.42);
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    std::shared_ptr<Lambertian> red  = std::make_shared<Lambertian>(Color(.65, .05, .05));

    ObjLoader::load(world, "./DIAMOND.obj", glass);

    cam.lookfrom = Point3(0, 0, 3);
    cam.lookat = Point3(0,0,0);

    cam.samples_per_pixel = 10;
    cam.max_get_color_depth = 100;

    cam.vertical_fov_rad = degrees_to_radians(90);

    //td::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
     //world.add(std::make_shared<Quad>(Point3(-5, 20, -5), Vec3(100,0,0), Vec3(0,0,100), light));

    std::shared_ptr<Lambertian> beige = std::make_shared<Lambertian>(Color(207.0/250.0, 185.0/250.0, 151.0/250.0));

    
    //std::shared_ptr<CheckerTexture> checkered_text_ptr = 
       // std::make_shared<CheckerTexture>(0.05, Color(0.1,0.1,0.1), Color(1,1,1));
    //std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(red);

    world.add(std::make_shared<Sphere>(Point3(0,-102,-1), 100, beige));
}

int main() {

    Camera camera = Camera(); 

    HittableList world;

    // std::shared_ptr<CheckerTexture> checkered_text_ptr = 
    //     std::make_shared<CheckerTexture>(0.05, Color(0.1,0.1,0.1), Color(1,1,1));

    // std::shared_ptr<Lambertian> mat_diffuse_ptr = std::make_shared<Lambertian>(Color(0.9, 0.9, 0.5));
    //std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    // std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(checkered_text_ptr);
    // std::shared_ptr<Metal> mat_metal_ptr = std::make_shared<Metal>(Color(0.5, 0.9, 0.9));
    // std::shared_ptr<Dielectric> mat_glass_ptr = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);


    // Three spheres
    // world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5, mat_diffuse_ptr, Vec3(0,0.5,0)));
    // world.add(std::make_shared<Sphere>(Point3(1,0,-1), 0.5, mat_metal_ptr));
    // world.add(std::make_shared<Sphere>(Point3(-1,0,-1), 0.5, mat_glass_ptr));
    // world.add(std::make_shared<Sphere>(Point3(-1,0,-1), -0.4, mat_glass_ptr));
    // world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100, mat_diffuse_background_ptr));

    // Checkered Spheres
    // world.add(std::make_shared<Sphere>(Point3(0,-10, 0), 9, std::make_shared<Lambertian>(checkered_text_ptr)));
    // world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 9, std::make_shared<Lambertian>(checkered_text_ptr)));


    // Quads
    // auto left_red     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    // auto back_green   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    // auto right_blue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    // auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    // auto lower_teal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(4,4,4));

    // // Quads
    // world.add(std::make_shared<Quad>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red));
    // world.add(std::make_shared<Quad>(Point3(-2,-2, 0), Vec3(5, 0, 0), Vec3(0, 5, 0), back_green));
    // world.add(std::make_shared<Quad>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    // world.add(std::make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), light));
    // world.add(std::make_shared<Quad>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal));
    
    //cornell_box(world, camera);
    //single_triangle(world, camera);
    //single_sphere(world,camera);
    HittableList lights; 
    //cornell_box(world, lights, camera);
    obj(world, lights, camera);

    world = HittableList(std::make_shared<BhvNode>(world.objects));

    camera.initialize(aspect_ratio, image_width);
    camera.render(world, lights);

    std::cerr << "ray and norm are perp: " << 
    HittableList::case1 << " t out of range: " << HittableList::case2 
    << " not in triangle: "  << HittableList::case3
    << " hit: " << HittableList::case4 << std::endl; 
}
