#pragma once
#include "Vec4.cpp"
#include "Vec3.cpp"

class Vertex {
public:
	Vec4 pos;
	Vec3 texCoord;
	Vertex(Vec4 pos, Vec3 texCoord) : pos(pos), texCoord(texCoord) {}
};