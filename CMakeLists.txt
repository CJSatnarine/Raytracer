cmake_minimum_required(VERSION 3.5.0)
project(Raytracer VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Raytracer 
    ImageGenerator.cpp
    camera.h
    colour.h
    hittable.h
    hittableList.h
    interval.h
    material.h
    ray.h
    rayTracer.h
    sphere.h
    vec3.h
)

