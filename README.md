# Raytracer
![mega_render](https://github.com/SZZZhang/Raytracer/assets/44757931/6d4fc9d3-5325-48bb-b9a7-b53038af53b1)

## Description
This raytracer creates images using monte carlo path tracing. Some features include: different materials (diffuse, dielelects/glass, metals), positionable camera, defocus blur, texture mapping, and obj loading.
To improve runtimes of the renders, basic geometries (spheres, triangles, quads) are organized in a bounding volume hierachy and multiple threads handle different rendering parts of the image. 

To run, run the script `buildnrun.sh`


Cornell box             |  Cornell box with glass sphere
:-------------------------:|:-------------------------:
![cornell_box](https://github.com/SZZZhang/Raytracer/assets/44757931/91cdfd1f-d34f-42f2-99e8-8c4317972058)  |  ![cornell_box_glass_sphere](https://github.com/SZZZhang/Raytracer/assets/44757931/73f13dac-5a02-48f4-97d1-8817e24ae34d)

## Run instructions
To run, run the script `./scripts/buildrun.sh`
<br />
Change the scene rendered by calling the a different function after the line `// Call scene function here` in `main.cpp`
<br />
Change the resolution by changing `image_width`. Increasing `cam.samples_per_pixel` will increase how realistic the render looks, it will also reduce noise. 
