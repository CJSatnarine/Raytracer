#include "rayTracer.h"
#include "bvh.h"
#include "camera.h"
#include "constantMedium.h"
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
    cam.background = colour(0.70, 0.80, 1.00);
    
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
    cam.background = colour(0.70, 0.80, 1.00);

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
    cam.background = colour(0.70, 0.80, 1.00);

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(0,0,12);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(hittableList(globe));
}

void funny() {
    int sphereXPos = 400;
    int sphereYPos = 200;
    int sphereZPos = 400;

    int cameraXPos = 3500;
    int cameraYPos = sphereYPos;
    int cameraZPos = 1200;

    hittableList world;

    auto material = make_shared<lambertian>(make_shared<imageTexture>("face.png"));
    world.add(make_shared<sphere>(point3(sphereXPos, sphereYPos, sphereZPos), 100, material));

    camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 4;
    cam.background = colour(0.70, 0.80, 1.00);

    cam.vFieldOfView = 40;
    cam.lookFrom = point3(cameraXPos, cameraYPos, cameraZPos);
    cam.lookAt = point3(sphereXPos, sphereYPos, sphereZPos);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
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
    cam.background = colour(0.70, 0.80, 1.00);

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
    cam.background = colour(0.70, 0.80, 1.00);

    cam.vFieldOfView = 80;
    cam.lookFrom = point3(0, 0, 9);
    cam.lookAt = point3(0, 0, 0);
    cam.vUp = vec3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.render(world);
}

void simpleLight() {
    hittableList world;

    auto perlinTexture = make_shared<noiseTexture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(perlinTexture)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(perlinTexture)));

    auto diffusionLight = make_shared<diffuseLight>(colour(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, diffusionLight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), diffusionLight));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = colour(0,0,0);

    cam.vFieldOfView = 20;
    cam.lookFrom = point3(26,3,6);
    cam.lookAt = point3(0,2,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void cornellBox() {
    hittableList world;

    auto red = make_shared<lambertian>(colour(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(colour(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(colour(0.12, 0.45, 0.15));
    auto light = make_shared<diffuseLight>(colour(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    // Boxes. 
    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotateY>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotateY>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    // Camera. 
    camera cam;
    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = colour(0, 0, 0);

    cam.vFieldOfView = 40;
    cam.lookFrom = point3(278, 278, -800);
    cam.lookAt = point3(278, 278, 0);
    cam.vUp = vec3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.render(world);
}

void cornellSmoke() {
    hittableList world;

    auto red   = make_shared<lambertian>(colour(.65, .05, .05));
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    auto green = make_shared<lambertian>(colour(.12, .45, .15));
    auto light = make_shared<diffuseLight>(colour(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotateY>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotateY>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constantMedium>(box1, 0.01, colour(0,0,0)));
    world.add(make_shared<constantMedium>(box2, 0.01, colour(1,1,1)));

    camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 600;
    cam.maxDepth = 50;
    cam.background = colour(0,0,0);

    cam.vFieldOfView = 40;
    cam.lookFrom = point3(278, 278, -800);
    cam.lookAt = point3(278, 278, 0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void finalScene(int imageWidth, int samplesPerPixel, int maxDepth) {
    hittableList boxes1;

    auto ground = make_shared<lambertian>(colour(0.48, 0.83, 0.53));

    int boxesPerSide = 20;
    for (int i = 0; i < boxesPerSide; i++) {
        for (int j = 0; j < boxesPerSide; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittableList world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuseLight>(colour(7, 7, 7));
    world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    auto centre1 = point3(400, 400, 200);
    auto centre2 = centre1 + vec3(30, 0, 0);
   auto sphereMaterial = make_shared<lambertian>(colour(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(centre1, centre2, 50, sphereMaterial));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(point3(0, 150, 145), 50, make_shared<metal>(colour(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constantMedium>(boundary, 0.2, colour(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constantMedium>(boundary, .0001, colour(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<imageTexture>("earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noiseTexture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittableList boxes2;
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<translate>(make_shared<rotateY>(make_shared<bvh_node>(boxes2), 15), vec3(-100,270,395)));

    camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = imageWidth;
    cam.samplesPerPixel = samplesPerPixel;
    cam.maxDepth = maxDepth;
    cam.background = colour(0,0,0);

    cam.vFieldOfView = 40;
    cam.lookFrom = point3(478, 278, -600);
    cam.lookAt = point3(278, 278, 0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void sceneChooser(int sceneToShow) {
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
        case 7:
            simpleLight();
            break;
        case 8:
            cornellBox();
            break;
        case 9: 
            cornellSmoke();
            break;
        case 10:
            finalScene(800, 10000, 40);
            break;
        default: 
            finalScene(400, 250, 40);
            break;
    }
}

int main(void) {

    // Choose a scene to render. 
    sceneChooser(1);
}