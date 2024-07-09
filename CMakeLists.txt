cmake_minimum_required(VERSION 3.5.0)
project(Raytracer VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Raytracer 
    main.cpp
    aabb.h
    bvh.h
    camera.h
    colour.h
    hittable.h
    hittableList.h
    interval.h
    material.h
    ray.h
    rayTracer.h
    rtw_stb_image.h
    sphere.h
    texture.h
    vec3.h
)

