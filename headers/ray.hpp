#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class Ray {
public:
    Ray();
    Ray(const Point3& origin, const Vec3& direction);

    const Point3& origin() const;
    const Vec3& direction() const;

    Point3 at(double t) const;

private:
    Point3 orig;
    Vec3 dir;
};

#endif // RAY_HPP