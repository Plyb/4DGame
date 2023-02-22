#pragma once
#include "Vec4.cpp"
#include <vector>

class Triangle {
public:
	Vec4 points[3];
	Triangle(Vec4 p1, Vec4 p2, Vec4 p3) : points{ p1, p2, p3 } {}
	Triangle(std::vector<Vec4> points) : points{ points[0], points[1], points[2] } {}
};