#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "utils.hpp"

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat);

    bool hit(const Ray& r, interval ray_t, HitRecord& rec) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat;
};

#endif // SPHERE_HPP
