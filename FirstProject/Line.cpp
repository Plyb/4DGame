#pragma once
#include "Vec4.cpp"
#include "Vertex.cpp"

class Line {
public:
	Vertex endpoints[2];
	Line(Vertex p1, Vertex p2) : endpoints{ p1, p2 } {}

	Vec4 getDir() const {
		return endpoints[1].pos - endpoints[0].pos;
	}

	Vertex tToPoint(double t) const {
		Vec4 pos = endpoints[0].pos + t * getDir();
		Vec3 texCoord = t * endpoints[1].texCoord + (1.0 - t) * endpoints[0].texCoord;
		return Vertex(pos, texCoord);
	}
};