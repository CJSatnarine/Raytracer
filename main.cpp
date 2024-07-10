#include "rayTracer.h"
#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

void bouncingSpheres(void) {
    // World. 
    hittableList world;

    // Code from the book. 
    auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));

                    auto center2 = center + vec3(0, randomDouble(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittableList(make_shared<bvh_node>(world));

    // Camera. 
    camera cam;
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    
    cam.vFieldOfView = 20;
    cam.lookFrom = point3(13,2,3);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 10;

    cam.render(world);
}

void checkeredSpheres(void) {
    hittableList world;

    auto checker = make_shared<checkerTexture>(0.32, colour(.9, .1, .1), colour(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));
    
    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0, 0, 0);
    cam.vUp = vec3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.render(world);
}

void earth(void) {
    auto earthTexture = make_shared<imageTexture>("earthmap.jpg");
    auto earthSurface = make_shared<lambertian>(earthTexture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earthSurface);

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(0,0,12);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(hittableList(globe));
}

void funny() {
    auto texture = make_shared<imageTexture>("face.png");
    auto surface = make_shared<lambertian>(texture);
    auto object = make_shared<sphere>(point3(0, 0, 0), 2, surface);

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(0,0,12);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(hittableList(object));
}

void perlinSpheres() {
    hittableList world;

    auto perlinTexture = make_shared<noiseTexture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(perlinTexture)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(perlinTexture)));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(13,2,3);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void quads() {
    hittableList world;

    // Materials. 
    auto leftRed = make_shared<lambertian>(colour(1.0, 0.2, 0.2));
    auto backGreen = make_shared<lambertian>(colour(0.2, 1.0, 0.2));
    auto rightBlue = make_shared<lambertian>(colour(0.2, 0.2, 1.0));
    auto upperOrange = make_shared<lambertian>(colour(1.0, 0.5, 0.0));
    auto lowerTeal = make_shared<lambertian>(colour(0.2, 0.8, 0.8));

    //Quads. 
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), leftRed));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), backGreen));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), rightBlue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upperOrange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lowerTeal));

    camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFieldOfView = 80;
    cam.lookFrom = point3(0, 0, 9);
    cam.lookAt = point3(0, 0, 0);
    cam.vUp = vec3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.render(world);
}

int main(void) {
    int sceneToShow = 6;

    switch (sceneToShow) {
        case 1: 
            bouncingSpheres();
            break;
        case 2:
            checkeredSpheres();
            break;
        case 3:
            earth();
            break;
        case 4:
            funny();
            break;
        case 5:
            perlinSpheres();
            break;
        case 6:
            quads();
            break;
    }
}