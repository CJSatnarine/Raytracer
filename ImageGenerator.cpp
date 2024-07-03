#include "rayTracer.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

int main(void) {
    // World. 
    hittableList world;

    auto groundMaterial = make_shared<lambertian>(colour(0.1, 0.0, 0.0));
    auto centreMaterial = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    auto leftMaterial   = make_shared<dielectric>(1.50);
    auto bubbleMaterial = make_shared<dielectric>(1.00 / 1.50);
    auto rightMaterial  = make_shared<metal>(colour(1.0, 0.0, 0.0), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, groundMaterial));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, centreMaterial));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, leftMaterial));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, bubbleMaterial));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, rightMaterial));

    // Camera. 
    camera cam;
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 1200;
    cam.samplesPerPixel = 500;
    cam.maxDepth = 50;
    
    cam.vFieldOfView = 90;
    cam.lookFrom = point3(-2,2,1);
    cam.lookAt = point3(0,0,-1);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 3.4;

    cam.render(world);
    
    return 0;
}