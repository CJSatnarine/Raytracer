#ifndef TEXTURE_H
#define TEXTURE_H

#include "rayTracer.h"

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

#endif