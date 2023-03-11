#pragma once
#include <vector>
#include "Tesseract.cpp"

class Scene {
public:
	std::vector<Tesseract> tesseracts;
	std::vector<Tetrahedron> tetrahedra;
	Scene() {
		for (int i = -3; i < 3; i++) {
			for (int j = -3; j < 3; j++) {
				tesseracts.push_back(Tesseract(Vec4(0, i, j, -0.5)));
			}
		}
		tetrahedra = getTetrahedra();
	}

private:
	std::vector<Tetrahedron> getTetrahedra() {
		std::vector<Tetrahedron> allTetras;
		for (int i = 0; i < tesseracts.size(); i++) {
			std::vector<Tetrahedron> tetras = tesseracts.at(i).getTetrahedra();
			allTetras.insert(allTetras.end(), tetras.begin(), tetras.end());
		}
		return allTetras;
	}
};