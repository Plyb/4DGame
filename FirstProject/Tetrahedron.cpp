#pragma once
#include "Vec4.cpp"
#include <vector>
#include "Line.cpp"
#include "Triangle.cpp"

class Tetrahedron {
public:
	Vertex points[4];
	Tetrahedron(Vertex p1, Vertex p2, Vertex p3, Vertex p4) : points{ p1, p2, p3, p4 } {}

	std::vector<Line> getLines() const {
		return std::vector<Line> { // TODO: turn this into a loop
			Line(points[0], points[1]),
			Line(points[1], points[2]),
			Line(points[2], points[3]),
			Line(points[3], points[0]),
			Line(points[0], points[2]),
			Line(points[1], points[3]),
		};
	}

	std::vector<Triangle> getTriangles() const {
		return std::vector<Triangle> {
			Triangle(points[0], points[1], points[2]),
			Triangle(points[0], points[3], points[1]),
			Triangle(points[1], points[3], points[2]),
			Triangle(points[0], points[2], points[3]),
		};
	}
};