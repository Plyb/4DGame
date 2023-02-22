#pragma once
#include "Vec4.cpp"

class Hyperplane {
public:
	Vec4 normal;
	double e;
	Hyperplane(Vec4 normal, double e) : normal(normal), e(e) {}
	Hyperplane(Vec4 normal, Vec4 point) : normal(normal), e(-(normal.dot(point))) {}
};