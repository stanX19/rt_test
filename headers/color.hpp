#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"
#include "utils.hpp"

using Color = Vec3;

inline double linear_to_gamma(double linear_component);
void write_color(std::ostream& out, const Color& pixel_color);

#endif