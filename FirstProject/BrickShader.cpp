#pragma once
#include "Camera.h"
#include "Shader.h"
#include <string>
#include <iostream>
#include "BrickTexture.cpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class BrickShader : public Shader {
public:
	Camera& camera;
	BrickShader(Camera& camera) : 
		camera(camera),
		Shader("./shader.vert", "./shader.frag")
	{
		loadBrickTexture();
	}

	void updateUniforms() {
		setMatrix("view", createViewTransform());
		setMatrix("projection", createProjectionTransform());
		setMatrix("dropDimension", createDropDimensionMat());
		setFloat("cameraPos", camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.w);
	}

private:
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

	int loadBrickTexture() {
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, texture);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		BrickTexture brickTexture;
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, brickTexture.sideLength, brickTexture.sideLength, brickTexture.sideLength, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickTexture.pixels);
		glGenerateMipmap(GL_TEXTURE_3D);

		glBindTexture(GL_TEXTURE_3D, texture);
		return texture;
	}
};