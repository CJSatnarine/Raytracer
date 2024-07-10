#ifndef CAMERA_H
#define CAMERA_H

#include "rayTracer.h"
#include "hittable.h"
#include "material.h"

class camera {
    public: 
        double aspectRatio = 1.0;   // Ratio of image width over height
        int imageWidth  = 100;      // Rendered image width in pixel count
        int samplesPerPixel = 10;   // Count of random samples for each pixel. 
        int maxDepth = 10;          // Maximum number of ray bounces into the scene.
        colour background;          // Scene background colour. 

        double vFieldOfView = 90;           // Vertical view angle (field of view)
        point3 lookFrom = point3(0, 0, 0);  // Point camera is looking from.  
        point3 lookAt = point3(0, 0, -1);   // Point camera is looking at. 
        vec3 vUp = vec3(0, 1, 0);           // Camera-relative "up" direction. 

        double defocusAngle = 0;    // Variation angle of rays through each pixel.
        double focusDistance = 10;  // Distance from camera lookFrom point to plane of perfect focus.

        void render(const hittable& world) {
            initialise();

            std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                
                for (int i = 0; i < imageWidth; i++) {
                    colour pixelColour(0, 0, 0);
                    for (int sample = 0; sample < samplesPerPixel; sample++) {
                        ray r = getRay(i, j);
                        pixelColour += rayColour(r, maxDepth, world);
                    }
                    writeColour(std::cout, pixelSamplesScale * pixelColour);
                }
            }

            std::clog << "\rDone.                 \n";
        }

    private: 
        int    imageHeight;         // Rendered image height
        double  pixelSamplesScale;  // Colour scale factor for a sum of pixel samples.
        point3  centre;             // Camera center
        point3  pixel00Location;    // Location of pixel 0, 0
        vec3    pixelDeltaU;        // Offset to pixel to the right
        vec3    pixelDeltaV;        // Offset to pixel below
        vec3    u, v, w;            // Camera frame basis vectors. 
        vec3    defocusDiskU;       // Defocus disk horizontal radius. 
        vec3    defocusDiskV;       // Defocus disk vertical radius. 

        void initialise() {
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            pixelSamplesScale = 1.0 / samplesPerPixel;

            centre = lookFrom;

            // Determine viewport dimensions.
            auto theta = degreesToRadians(vFieldOfView);
            auto h = tan(theta / 2);
            auto viewportHeight = 2 * h * focusDistance;
            auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);

            // Calculate the u, v, and w unit basis vectors for the camera coordinate frame.
            w = unitVector(lookFrom - lookAt);
            u = unitVector(cross(vUp, w));
            v = cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewportU = viewportWidth * u;    // Vector across viewport horizontal edge
            vec3 viewportV = viewportHeight * -v;  // Vector down viewport vertical edge

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            // Calculate the location of the upper left pixel.
            auto viewportUpperLeft = centre - (focusDistance * w) - viewportU/2 - viewportV/2;
            pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

            // Calculate the camera defocus disk basis vectors. 
            auto defocusRadius = focusDistance * tan(degreesToRadians(defocusAngle / 2));
            defocusDiskU = u * defocusRadius;
            defocusDiskV = v * defocusRadius;
        }

        ray getRay(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled point around the pixel location i, j.

            auto offset = sampleSquare();
            auto pixelSample = pixel00Location
                            + ((i + offset.x()) * pixelDeltaU)
                            + ((j + offset.y()) * pixelDeltaV);

            auto rayOrigin = (defocusAngle <= 0) ? centre : defocusDiskSample();
            auto rayDirection = pixelSample - rayOrigin;
            auto rayTime = randomDouble();
            return ray(rayOrigin, rayDirection, rayTime);
        }

        vec3 sampleSquare() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
        }

        point3 defocusDiskSample() const {
            // Returns a random point in the camera defocus disk. 
            auto p = randomInUnitDisk();
            return centre + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
        }

        colour rayColour(const ray& r, int depth, const hittable& world) const {
            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (depth <= 0) return colour(0, 0, 0);

            hitRecord rec;

            // if hte ray hits nothing, return the background colour. 
            if (!world.hit(r, interval(0.001, infinity), rec)) return background;

            ray scattered;
            colour attenuation;
            colour colourFromEmission = rec.mat->emitted(rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, attenuation, scattered)) return colourFromEmission;

            colour colourFromScatter = attenuation * rayColour(scattered, depth - 1, world);
            return colourFromEmission + colourFromScatter;
        }
};
#endif