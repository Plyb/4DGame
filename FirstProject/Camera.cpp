#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec4 pos = glm::vec4(0.0f, 0.0f, 3.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	float psi = 0.0f; 
};