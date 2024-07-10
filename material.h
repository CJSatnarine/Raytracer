#ifndef MATERIAL_H
#define MATERIAL_H

#include "rayTracer.h"
#include "texture.h"

class hitRecord;

class material {
    public:
        virtual ~material() = default;

        virtual colour emitted(double u, double v, const point3& p) const {
            return colour(0,0,0);
        }

        virtual bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian : public material {
  private:
    shared_ptr<texture> tex;

  public:
    lambertian(const colour& albedo) : tex(make_shared<solidColour>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const override {
        auto scatterDirection = rec.normal + randomUnitVector();

        // Catch degenerate scatter direction. 
        if (scatterDirection.nearZero()) scatterDirection = rec.normal;

        scattered = ray(rec.p, scatterDirection, rIn.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }
};

class metal : public material {
    private:
        colour albedo;
        double fuzz;

    public: 
        metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(rIn.direction(), rec.normal);
            reflected = unitVector(reflected) + (fuzz * randomUnitVector());
            scattered = ray(rec.p, reflected, rIn.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

class dielectric : public material {
    private: 
        // Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media. 
        double refractionIndex;

        static double reflectance(double cosine, double refractionIndex) {
            auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
    
    public: 
        dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

        bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const override {
            attenuation = colour(1.0, 1.0, 1.0);
            double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

            vec3 unitDirection = unitVector(rIn.direction());
            double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
            double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

            bool cannotRefract = ri * sinTheta > 1.0;
            vec3 direction;

            if (cannotRefract || reflectance(cosTheta, ri) > randomDouble()) {
                direction = reflect(unitDirection, rec.normal);
            } else {
                direction = refract(unitDirection, rec.normal, ri);
            }

            scattered = ray(rec.p, direction, rIn.time());
            return true;
        }
};

class diffuseLight : public material {
  public:
    diffuseLight(shared_ptr<texture> tex) : tex(tex) {}
    diffuseLight(const colour& emit) : tex(make_shared<solidColour>(emit)) {}

    colour emitted(double u, double v, const point3& p) const override {
        return tex->value(u, v, p);
    }

  private:
    shared_ptr<texture> tex;
};

#endif