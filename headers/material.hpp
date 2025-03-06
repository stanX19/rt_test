#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "color.hpp"
#include "interval.hpp"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const;
};

class Lambertian : public Material {
public:
    Lambertian(const Color& albedo);

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
};

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz = 0);

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double refraction_index = 1.0, const Color& albedo = Color(1, 1, 1), double fuzz = 0);

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
    double fuzz;
    double refraction_index;

    static double reflectance(double cosine, double refraction_index);
};

#endif // MATERIAL_HPP
