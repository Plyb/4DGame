#include "Camera.h"
#include <GLFW/glfw3.h>

void Camera::update(float deltaTime, std::function<bool(int)> keyPressed) {
	float cameraSpeed = deltaTime * CAMERA_SPEED;

	if (keyPressed(GLFW_KEY_W)) {
		pos += cameraSpeed * front;
	}
	if (keyPressed(GLFW_KEY_S)) {
		pos -= cameraSpeed * front;
	}
	if (keyPressed(GLFW_KEY_A)) {
		pos -= side * cameraSpeed;
	}
	if (keyPressed(GLFW_KEY_D)) {
		pos += side * cameraSpeed;
	}
	if (keyPressed(GLFW_KEY_Q)) {
		psi += cameraSpeed;
	}
	if (keyPressed(GLFW_KEY_E)) {
		psi -= cameraSpeed;
	}
}