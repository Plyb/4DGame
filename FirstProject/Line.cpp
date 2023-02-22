#pragma once
#include "Vec4.cpp"

class Line {
public:
	Vec4 endpoints[2];
	Line(Vec4 p1, Vec4 p2) : endpoints{ p1, p2 } {}

	Vec4 getDir() const {
		return endpoints[1] - endpoints[0];
	}

	Vec4 tToPoint(double t) const {
		return endpoints[0] + t * getDir();
	}
};