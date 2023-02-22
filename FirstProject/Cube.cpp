#pragma once
#include "Vec4.cpp"
#include <vector>
#include "Tetrahedron.cpp"

class Cube {
public:
	Vec4 lowestCorner;
	int dimToExclude;
	Cube(Vec4 lowestCorner, int dimToExclude) : lowestCorner(lowestCorner),
		dimToExclude(dimToExclude) {}

	std::vector<Tetrahedron> getTetrahedra() {
		Vec4 corners[8] = {
			makeCorner(std::vector<double> { 0.0f, 0.0f, 0.0f, 0.0f }),
			makeCorner(std::vector<double> { 1.0f, 0.0f, 0.0f, 0.0f }),
			makeCorner(std::vector<double> { 0.0f, 1.0f, 0.0f, 0.0f }),
			makeCorner(std::vector<double> { 1.0f, 1.0f, 0.0f, 0.0f }),
			makeCorner(std::vector<double> { 0.0f, 0.0f, 1.0f, 0.0f }),
			makeCorner(std::vector<double> { 1.0f, 0.0f, 1.0f, 0.0f }),
			makeCorner(std::vector<double> { 0.0f, 1.0f, 1.0f, 0.0f }),
			makeCorner(std::vector<double> { 1.0f, 1.0f, 1.0f, 0.0f })
		};

		return std::vector<Tetrahedron> {
			Tetrahedron(corners[0], corners[1], corners[2], corners[4]),
			Tetrahedron(corners[2], corners[4], corners[6], corners[7]),
			Tetrahedron(corners[1], corners[4], corners[5], corners[7]),
			Tetrahedron(corners[1], corners[2], corners[3], corners[7]),
			Tetrahedron(corners[1], corners[2], corners[4], corners[7])
		};
	}

private:
	Vec4 makeCorner(std::vector<double> components) {
		components.insert(components.begin() + dimToExclude, 0.0f);
		return Vec4(components) + lowestCorner;
	}
};