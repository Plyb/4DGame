#pragma once
#include <optional>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>

class Vec3 {
public:
	double u;
	double v;
	double t;
	Vec3(double u, double v, double t) : u(u), v(v), t(t) {}
	Vec3(glm::vec3 v) : u(v.x), v(v.y), t(v.z) {}
	Vec3(std::vector<double> components) : u(components[0]), v(components[1]),
		t(components[2]) {}

	Vec3 operator+(const Vec3 o) const {
		return Vec3(u + o.u, v + o.v, t + o.t);
	}

	Vec3 operator-(const Vec3 o) const {
		return Vec3(u - o.u, v - o.v, t - o.t);
	}

	friend Vec3 operator*(double lhs, const Vec3 rhs) {
		return Vec3(lhs * rhs.u, lhs * rhs.v, lhs * rhs.t);
	}

	Vec3 operator/(double o) {
		return Vec3(u / o, v / o, t / o);
	}

	operator std::string() {
		std::stringstream res;
		res << u << ' ' << v << ' ' << t;
		return res.str();
	}

	double dot(const Vec3 o) const {
		return u * o.u + v * o.v + t * o.t;
	}

	double magnitude() const {
		return sqrt(u * u + v * v + t * t);
	}
};