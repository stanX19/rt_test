#include "material.hpp"
#include "utils.hpp"

bool Material::scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
    (void)r_in;
    (void)rec;
    (void)attenuation;
    (void)scattered;
    return false;
}

Lambertian::Lambertian(const Color& albedo) : albedo(albedo) {}

bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
    (void)r_in;
    Vec3 scatter_direction = rec.normal + random_unit_vector();

    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}

Metal::Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

bool Metal::scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
    Vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

Dielectric::Dielectric(double refraction_index, const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz), refraction_index(refraction_index) {}

bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
    double ri;
    attenuation = albedo;
    ri = rec.front_face ? 1.0 / refraction_index : refraction_index;
    Vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
        if (rec.front_face)
            attenuation = Color(1, 1, 1);
        direction = reflect(unit_direction, rec.normal);
    } else {
        direction = refract(unit_direction, rec.normal, ri);
    }

    direction = unit_vector(direction) + (fuzz * random_unit_vector());
    scattered = Ray(rec.p, direction);
    return true;
}

double Dielectric::reflectance(double cosine, double refraction_index) {
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}