#include "vec3.hpp"

Vec3::Vec3() : e{0, 0, 0} {}
Vec3::Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

double Vec3::x() const { return e[0]; }
double Vec3::y() const { return e[1]; }
double Vec3::z() const { return e[2]; }

Vec3 Vec3::operator-() const
{
    return Vec3(-e[0], -e[1], -e[2]);
}

double Vec3::operator[](int i) const
{
    return e[i];
}

double &Vec3::operator[](int i)
{
    return e[i];
}

Vec3 &Vec3::operator+=(const Vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

Vec3 &Vec3::operator*=(double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

Vec3 &Vec3::operator/=(double t)
{
    return (*this *= 1 / t);
}

double Vec3::length() const
{
    return std::sqrt(length_squared());
}

double Vec3::length_squared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

Vec3 Vec3::random()
{
    return Vec3(random_double(), random_double(), random_double());
}

Vec3 Vec3::random(double min, double max)
{
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

bool Vec3::near_zero() const
{
    auto s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

Vec3 operator*(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

Vec3 operator*(double t, const Vec3 &v)
{
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

Vec3 operator*(const Vec3 &v, double t)
{
	return t * v;
}

Vec3 operator/(const Vec3 &v, double t)
{
	return (1 / t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

Vec3 cross(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

Vec3 unit_vector(const Vec3 &v)
{
	return v / v.length();
}

Vec3 random_unit_vector()
{
	while (true)
	{
		auto p = Vec3::random(-1, 1);
		auto lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

Vec3 random_on_hemisphere(const Vec3 &normal)
{
	Vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
	return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3& uv, const Vec3& un, double etai_over_etat) {
    double cos_theta = std::fmin(dot(-uv, un), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*un);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * un;
    return r_out_perp + r_out_parallel;
}

Vec3 pow(const Vec3&v, double p) {
	return Vec3(std::pow(v[0], p), std::pow(v[1], p), std::pow(v[2], p));
}