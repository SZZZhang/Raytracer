#include <cmath>
#include <memory>
#include <string>
#include <thread>

#include "Vec3.h"
#include "Sphere.h"
#include "Quad.h"
#include "HittableList.h"
#include "utils.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Triangle.h"
#include "ObjLoader.h"

double aspect_ratio = 1; //16.0 / 9.0;
int image_width = 1000;

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
    
    world.add(Quad::get_box(Point3(130, 0, 65), Vec3(165, 0, 0), Vec3(0,0, 230), 165, white, -18));
    world.add(Quad::get_box(Point3(265, 0, 295), Vec3(165, 0, 0), Vec3(0, 0, 165), 330, white, 18));

    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);

    cam.samples_per_pixel = 10;//4096;//1000;
    cam.max_get_color_depth = 200;
}

void cornell_box_glass_sphere(HittableList& world, HittableList& lights, Camera& cam) {
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
    auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    world.add(std::make_shared<Sphere>(Point3(190,90,190), 90, glass));

    world.add(Quad::get_box(Point3(265, 0, 295), Vec3(165, 0, 0), Vec3(0, 0, 165), 330, white, 18));

    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);

    cam.samples_per_pixel = 4096;//1000;
    cam.max_get_color_depth = 200;
    image_width = 1000;
}

void mega_render(HittableList& world, HittableList& lights, Camera& cam) {
    std::shared_ptr<Lambertian> pastel_green  = std::make_shared<Lambertian>(Color(.65, .05, .05));
    std::shared_ptr<Lambertian> pastel_purple  = std::make_shared<Lambertian>(Color(197.0/255.0, 177.0/255.0, 225.0/255.0));
    std::shared_ptr<Lambertian> pastel_blue  = std::make_shared<Lambertian>(Color(253.0/255.0, 253.0/255.0, 150.0/255.0));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    std::shared_ptr<Lambertian> blue = std::make_shared<Lambertian>(Color(0.5,0.9,0.9));
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    std::shared_ptr<Lambertian> red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
    std::shared_ptr<CheckerTexture> checkered_text_ptr = 
       std::make_shared<CheckerTexture>(0.05, Color(0.1,0.1,0.1), Color(1,1,1));
    std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(checkered_text_ptr);

    world.add(std::make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), pastel_purple));
    world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), pastel_purple));
    world.add(std::make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), mat_diffuse_background_ptr));
    world.add(std::make_shared<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), pastel_blue));
    world.add(std::make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), pastel_blue));

    auto light_quad = std::make_shared<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light);
    lights.add(light_quad);
    world.add(light_quad);

    // metal sphere
    std::shared_ptr<Metal> mat_metal_ptr = std::make_shared<Metal>(Color(0.5, 0.9, 0.9));
    world.add(std::make_shared<Sphere>(Point3(150,100,150), 100.0, mat_metal_ptr));

    // moving sphere 
    world.add(std::make_shared<Sphere>(Point3(350 ,375,100), 50.0, blue, Vec3(15, 15, 0)));

    // glass gem
    auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    ObjLoader::load(world, "./Assets/Diamond2.obj", glass);

    // diffuse box: 
    world.add(Quad::get_box(Point3(155, 0, 295), Vec3(165, 0, 0), Vec3(0, 0, 165), 330, blue, 18));

    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);

    cam.samples_per_pixel = 200;//1000;
    cam.max_get_color_depth = 200;

    image_width = 1000;
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

void skull_obj(HittableList& world, HittableList& lights, Camera& cam) {
    auto glass = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    std::shared_ptr<Lambertian> purple   = std::make_shared<Lambertian>(Color(236.0/255.0, 230.0/255.0, 255.0/255.0));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    std::shared_ptr<Lambertian> beige = std::make_shared<Lambertian>(Color(207.0/255.0, 185.0/255.0, 151.0/255.0));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
    std::shared_ptr<Metal> mat_metal_ptr = std::make_shared<Metal>(Color(0.5, 0.9, 0.9));
    std::shared_ptr<Lambertian> pastel_purple  = std::make_shared<Lambertian>(Color(197.0/255.0, 177.0/255.0, 225.0/255.0));
    std::shared_ptr<Lambertian> pastel_blue  = std::make_shared<Lambertian>(Color(167.0/255.0, 199.0/255.0, 231.0/255.0));
    std::shared_ptr<Lambertian> pastel_turquoise  = std::make_shared<Lambertian>(Color(180.0/255.0, 211.0/255.0, 178.0/255.0));
    std::shared_ptr<CheckerTexture> checkered_text_ptr = 
       std::make_shared<CheckerTexture>(0.05, Color(0.1,0.1,0.1), Color(1,1,1));

    ObjLoader::load(world, "./Assets/Skull.obj", glass);
    auto background_sphere = std::make_shared<Sphere>(Point3(-10,0,10), 10, pastel_blue);
    world.add(background_sphere);

    world.add(std::make_shared<Sphere>(Point3(0,-101,-1), 100, pastel_turquoise));
    world.add(std::make_shared<Sphere>(Point3(4,0,4), 2, pastel_purple));
    world.add(std::make_shared<Sphere>(Point3(-4,0,4), 2, pastel_purple));

    auto light_quad = std::make_shared<Quad>(Point3(-10, 20, -10), Vec3(10,0,0), Vec3(0,0,10), light);
    world.add(light_quad);
    lights.add(light_quad);

    cam.lookfrom = Point3(4, 0, -4);
    cam.lookat = Point3(0,0,0);

    cam.samples_per_pixel = 1000;
    image_width = 1000;
}

void three_spheres(HittableList& world, HittableList& lights, Camera& cam) {
    std::shared_ptr<Lambertian> mat_diffuse_ptr = std::make_shared<Lambertian>(Color(0.9, 0.9, 0.5));
    std::shared_ptr<Lambertian> mat_diffuse_background_ptr = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    std::shared_ptr<Metal> mat_metal_ptr = std::make_shared<Metal>(Color(0.5, 0.9, 0.9));
    std::shared_ptr<Dielectric> mat_glass_ptr = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);

    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5, mat_diffuse_ptr, Vec3(0,0.5,0)));
    world.add(std::make_shared<Sphere>(Point3(1,0,-1), 0.5, mat_metal_ptr));
    world.add(std::make_shared<Sphere>(Point3(-1,0,-1), 0.5, mat_glass_ptr));
    world.add(std::make_shared<Sphere>(Point3(-1,0,-1), -0.4, mat_glass_ptr));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100, mat_diffuse_background_ptr));
}

int main() {
    Camera camera = Camera(); 

    HittableList world;
    HittableList lights; 

    // Call scene function here
    mega_render(world, lights, camera);

    world = HittableList(std::make_shared<BvhNode>(world.objects));

    camera.initialize(aspect_ratio, image_width);

    int num_of_threads = 8;
    int image_height = camera.image_height;
    std::vector<std::thread> threads;
    size_t buff_len = image_width * image_height * 3;
    unsigned char buff[buff_len];

    for (int i = 0; i < num_of_threads; ++i) {
        int rows = i == num_of_threads - 1 ? image_height - (num_of_threads - 1) * (image_height/num_of_threads) : 
            (image_height/num_of_threads);
        int rows_start = i * (image_height/num_of_threads);
        int rows_end = rows_start + rows;
        
        threads.emplace_back(std::thread([&](int r_st, int r_ed){ camera.render(world, lights, r_st, r_ed, buff);}, rows_start, rows_end));
    }

    for (int i = 0; i < num_of_threads; ++i) {
        threads[i].join();
    }

    camera.write_colors_to_stream(std::cout, buff);
}
