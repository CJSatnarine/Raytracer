#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "rayTracer.h"
#include "rtw_stb_image.h"

class texture {
    public: 
        virtual ~texture() = default;

        virtual colour value(double u, double v, const point3& p) const = 0;

};

class solidColour : public texture {
    public: 
        solidColour(const colour& albedo) : albedo(albedo) {}

        solidColour(double red, double green, double blue) : solidColour(colour(red, green, blue)) {}

        colour value(double u, double v, const point3& p) const override {
            return albedo;
        }

    private: 
        colour albedo;
};

class checkerTexture : public texture {
    public:
        checkerTexture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd) : invScale(1.0 / scale), even(even), odd(odd) {}

        checkerTexture(double scale, const colour& c1, const colour& c2) : invScale(1.0 / scale), even(make_shared<solidColour>(c1)), odd(make_shared<solidColour>(c2)) {}

        colour value(double u, double v, const point3& p) const override {
            auto xInteger = int(std::floor(invScale * p.x()));
            auto yInteger = int(std::floor(invScale * p.y()));
            auto zInteger = int(std::floor(invScale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double invScale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

class imageTexture : public texture {
    public:
        imageTexture(const char* filename) : image(filename) {}

        colour value(double u, double v, const point3& p) const override {
            // If no texture deta, then return solid cyan. 
            if (image.height() <= 0) return colour(0, 1, 1);

            // Clamp input texture coordinates to [0, 1] x [1, 0]. 
            u = interval(0, 1).clamp(u);
            v = 1.0 - interval(0,1).clamp(v); // Flip v to image coordinates. 

            auto i = int(u * image.width());
            auto j = int(v * image.height());
            auto pixel = image.pixelData(i, j);

            auto colourScale = 1.0 / 255.0;
            return colour(colourScale * pixel[0], colourScale * pixel[1], colourScale * pixel[2]);
        }

    private:
        rtwImage image;
};

class noiseTexture : public texture {
  public:
    noiseTexture() {}

    noiseTexture(double scale) : scale(scale) {}

    colour value(double u, double v, const point3& p) const override {
        return colour(1, 1, 1) * noise.turbulance(p, 7);
    }

  private:
    perlin noise;
    double scale;
};

#endif