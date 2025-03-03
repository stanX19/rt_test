#ifndef RAY_HPP
#define RAY_HPP

#include "vulkan_test.hpp"

class Ray {
  public:
    Ray() {}

    Ray(const point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin() const  { return orig; }
    const Vec3& direction() const { return dir; }

    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    Vec3 dir;
};

#endif