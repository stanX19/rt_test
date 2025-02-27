#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "material.hpp"
#include "hittable.hpp"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(
		const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
	{
		(void)r_in;
		(void)rec;
		(void)attenuation;
		(void)scattered;
		return false;
	}
};

class Lambertian : public Material
{
public:
	Lambertian(const Color &albedo) : albedo(albedo) {}

	bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered)
		const override
	{
		(void)r_in;
		Vec3 scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color &albedo, double fuzz = 0) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered)
		const override
	{
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double refraction_index=1.0, const Color &albedo=Color(1, 1, 1), double fuzz = 0) : albedo(albedo), fuzz(fuzz), refraction_index(refraction_index) {}

	bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered)
		const override
	{
		double ri;

		if (rec.front_face) {
			ri = 1.0 / refraction_index;
			attenuation = Color(1,1,1);
		} else {
			ri = refraction_index;
			double distance = r_in.direction().length() * rec.t;
			attenuation = pow(albedo, distance);
		}
		Vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		Vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);
		direction = unit_vector(direction) + (fuzz * random_unit_vector());

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	Color albedo;
	double fuzz;
	// Refractive index in vacuum or air, or the ratio of the material's refractive index over
	// the refractive index of the enclosing media
	double refraction_index;

	static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};

#endif