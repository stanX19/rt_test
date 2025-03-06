#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "interval.hpp"

class Material;

class HitRecord {
public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;

    void set_face_normal(const Ray& r, const Vec3& outward_normal);
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, interval ray_t, HitRecord& rec) const = 0;
};

#endif // HITTABLE_HPP