#pragma once
#include "Vec4.cpp"
#include <vector>
#include "Vertex.cpp"

class Triangle {
public:
	Vertex points[3];
	Triangle(Vertex p1, Vertex p2, Vertex p3) : points{ p1, p2, p3 } {}
	Triangle(std::vector<Vertex> points) : points{ points[0], points[1], points[2] } {}
};