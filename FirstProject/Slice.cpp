#pragma once
#include <optional>
#include <functional>
#include "Vec4.cpp"
#include "Line.cpp"
#include "Hyperplane.cpp"
#include "Triangle.cpp"
#include "Tetrahedron.cpp"

inline std::optional<Vec4> intersect(Line line, Hyperplane hyperplane) {
	Vec4 p_n = hyperplane.normal;
	Vec4 l_0 = line.endpoints[0];
	Vec4 l_d = line.getDir();
	
	double numerator = l_0.dot(p_n) + hyperplane.e;
	if (numerator == 0) {
		// The first endpoint lies in the hyperplane
		return l_0;
	}

	double denominator = p_n.dot(l_d);
	if (denominator == 0) {
		// The line is parallel to the hyperplane
		return std::nullopt;
	}

	double t = -(p_n.dot(l_0) + hyperplane.e) / denominator;
	if (t < 0 || t > 1) {
		// The *line* intersects, but ouside the line segment
		return std::nullopt;
	}

	return line.tToPoint(t);
}

inline std::vector<Triangle> intersect(Tetrahedron tetrahedron, Hyperplane hyperplane) {
	std::vector<Vec4> intersectionPoints;
	std::vector<Line> tetrahedronLines = tetrahedron.getLines();
	for (int i = 0; i < tetrahedronLines.size(); i++) {
		Line line = tetrahedronLines[i];
		std::optional<Vec4> point = intersect(line, hyperplane);
		if (point.has_value()) {
			intersectionPoints.push_back(point.value());
		}
	}

	if (intersectionPoints.size() == 3) {
		// The tetrahedron is askew to the hyperplane, so there is only one tri of intersection
		return std::vector<Triangle> {
			Triangle(intersectionPoints)
		};
	}
	else if (intersectionPoints.size() == 4) {
		// The tetrahedron is askew to the hyperplane and is rotated to produce a quadrilateral
		
		// We need to find a diagonal of the quad first. Most straightforward way is to find the longest distance
		// There's almost certainly a better way to do this, but this works for now
		float longestDistance = 0.0f;
		int a, b;
		for (int i = 0; i < intersectionPoints.size(); i++) {
			for (int j = i + 1; j < intersectionPoints.size(); j++) {
				float distance = (intersectionPoints[i] - intersectionPoints[j]).magnitude();
				if (distance > longestDistance) {
					longestDistance = distance;
					a = i;
					b = j;
				}
			}
		}

		// find the other two indices
		int c = -1;
		int d = -1;
		for (int i = 0; i < intersectionPoints.size(); i++) {
			if (i != a && i != b) {
				if (c == -1) {
					c = i;
				}
				else {
					d = i;
				}
			}
		}

		return std::vector<Triangle> {
			Triangle(intersectionPoints[a], intersectionPoints[c], intersectionPoints[b]),
			Triangle(intersectionPoints[a], intersectionPoints[b], intersectionPoints[d])
		};
	}
	else if (intersectionPoints.size() == 6) {
		// The tetrahedron is entirely embedded in the hyperplane, so return all 4 triangles that make up the tetra
		return tetrahedron.getTriangles();
	}
	else {
		// The tetrahedron does not intersect, or only intersects at one point, so return no triangles
		return std::vector<Triangle>{};
	}
}

inline std::vector<Triangle> getTriangles(std::vector<Tetrahedron> tetrahedra, Hyperplane hyperplane) {
	std::vector<Triangle> allTriangles;
	for (int i = 0; i < tetrahedra.size(); i++) {
		Tetrahedron tetrahedron = tetrahedra[i];
		std::vector<Triangle> triangles = intersect(tetrahedron, hyperplane);
		allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());
	}
	return allTriangles;
}


inline std::vector<float> getVertices(std::vector<Tetrahedron> tetrahedra, Hyperplane hyperplane) {
	std::vector<Triangle> triangles = getTriangles(tetrahedra, hyperplane);
	std::vector<float> vertices;
	for (int i = 0; i < triangles.size(); i++) {
		Triangle triangle = triangles[i];
		for (int j = 0; j < sizeof(triangle.points) / sizeof(Vec4); j++) {
			Vec4 point = triangle.points[j];
			vertices.push_back((float)point.x);
			vertices.push_back((float)point.y);
			vertices.push_back((float)point.z);
			vertices.push_back((float)point.w); // In OpenGL, w is the fourth comp of vec4
		}
	}
	return vertices;
}