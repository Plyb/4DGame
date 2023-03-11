#pragma once
#include "Camera.cpp"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class BrickShader : public Shader {
public:
	Camera camera;
	BrickShader(Camera camera) : 
		camera(camera),
		Shader((RES_PATH + "shader.vert").c_str(), (RES_PATH + "shader.frag").c_str())
	{}

	void updateUniforms() {
		setMatrix("view", createViewTransform());
		setMatrix("projection", createProjectionTransform());
		setMatrix("dropDimension", createDropDimensionMat());
		setFloat("cameraPos", camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.w);
	}

private:
	const std::string RES_PATH = "./";
	glm::mat4 createDropDimensionMat() {
		float testMat4[16] = {
			1.0f, 0.0f, 0.0f,					0.0f,
			0.0f, 1.0f, 0.0f,					0.0f,
			0.0f, 0.0f, cos(glm::radians(camera.psi)), 0.0f,
			0.0f, 0.0f, sin(glm::radians(camera.psi)), 0.0f
		};
		return glm::make_mat4(testMat4);
	}

	glm::mat4 createViewTransform() {
		glm::vec3 threeCameraPos(camera.pos.x, camera.pos.y, cos(glm::radians(camera.psi)) * camera.pos.z + sin(glm::radians(camera.psi)) * camera.pos.w);
		glm::vec3 cameraFront(
			cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)),
			sin(glm::radians(camera.pitch)),
			sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch))
		);
		glm::mat4 view = glm::lookAt(threeCameraPos, threeCameraPos + cameraFront, camera.up);
		return view;
	}

	glm::mat4 createProjectionTransform() {
		return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	}
};