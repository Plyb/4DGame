#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <functional>

class Camera {
public:
	const float CAMERA_SPEED = 2.5f;

	glm::vec4 pos = glm::vec4(0.0f, 0.0f, 3.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	float psi = 0.0f;

	void onMousePosChange(double xoffset, double yoffset) {
		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		front = computeFront();
		side = computeSide();
	}

	void update(float deltaTime, std::function<bool(int)> keyPressed, std::function<void(void)> fillVbo) {
		float cameraSpeed = deltaTime * CAMERA_SPEED;

		if (keyPressed(GLFW_KEY_W) == GLFW_PRESS) {
			pos += cameraSpeed * front;
		}
		if (keyPressed(GLFW_KEY_S) == GLFW_PRESS) {
			pos -= cameraSpeed * front;
		}
		if (keyPressed(GLFW_KEY_A) == GLFW_PRESS) {
			pos -= side * cameraSpeed;
		}
		if (keyPressed(GLFW_KEY_D) == GLFW_PRESS) {
			pos += side * cameraSpeed;
		}
		if (keyPressed(GLFW_KEY_Q) == GLFW_PRESS) {
			psi += 0.1f;
			fillVbo();
		}
		if (keyPressed(GLFW_KEY_E) == GLFW_PRESS) {
			psi -= 0.1f;
			fillVbo();
		}
	}


private:
	glm::vec4 front = computeFront();
	glm::vec4 side = computeSide();

	glm::vec4 computeFront() {
		return glm::vec4(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * cos(glm::radians(psi)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * sin(glm::radians(psi))
		);
	}

	glm::vec4 computeSide() {
		return glm::normalize(glm::vec4(
			-sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
			0.0f,
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * cos(glm::radians(psi)),
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * sin(glm::radians(psi))
		));
	}
};