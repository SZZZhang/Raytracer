#include <cmath>
#include <memory>

#include "Vec3.h"
#include "Sphere.h"
#include "HittableList.h"
#include "utils.h"
#include "Camera.h"

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
int main() {

    Camera camera = Camera(aspect_ratio, image_width); 

    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));
    
    camera.render(world);
}
