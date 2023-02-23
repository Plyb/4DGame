#pragma once
#include <optional>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>

class Vec4 {
public:
	double x;
	double y;
	double z;
	double w;
	Vec4(double x, double y, double z, double w) : w(w), x(x), y(y), z(z) {}
	Vec4(glm::vec4 v) : w(v.w), x(v.x), y(v.y), z(v.z) {}
	Vec4(std::vector<double> components) : x(components[0]), y(components[1]),
		z(components[2]), w(components[3]) {}

	Vec4 operator+(const Vec4 o) const {
		return Vec4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	Vec4 operator-(const Vec4 o) const {
		return Vec4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	friend Vec4 operator*(double lhs, const Vec4 rhs) {
		return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}

	Vec4 operator/(double o) const {
		return Vec4(x / o, y / o, z / o, w / o);
	}

	operator std::string() {
		std::stringstream res;
		res << x << ' ' << y << ' ' << z << ' ' << w;
		return res.str();
	}

	double dot(const Vec4 o) const {
		return x * o.x + y * o.y + z * o.z + w * o.w;
	}

	double magnitude() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	Vec4 normalize() const {
		return (*this) / magnitude();
	}
};

