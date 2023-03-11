#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>

#include "Shader.h"

#include <iostream>
#include <vector>
#include "Hyperplane.cpp"
#include "Tetrahedron.cpp"
#include "Slice.cpp"
#include "Cube.cpp"
#include "Tesseract.cpp"
#include "BrickTexture.cpp"
#include "Scene.cpp"
#include "Camera.cpp"
#include "BrickShader.cpp"

Camera camera;

const int vertexSize = 7;
float proportion = 0.2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;
unsigned int VBO = 0;
int numVerts = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void fillVbo() {
	std::vector<Tetrahedron> tetras = Scene().getTetrahedra();
	Hyperplane hp(Vec4(0.0f, 0.0f, sin(glm::radians(camera.psi)), -cos(glm::radians(camera.psi))), Vec4(camera.pos));
	std::vector<float> vertexVector = getVertices(tetras, hp);
	float* vertices = vertexVector.data();
	numVerts = vertexVector.size() / vertexSize;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertices, GL_STATIC_DRAW);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.onMousePosChange(xoffset, yoffset);
}

void processInput(GLFWwindow* window, const Shader& shader) {
	auto keyPressed = [&window](int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	};

	if (keyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	camera.update(deltaTime, keyPressed, fillVbo);

	if (keyPressed(GLFW_KEY_Q) || keyPressed(GLFW_KEY_E)) {
		fillVbo();
	}

}

int loadBrickTexture(int textureUnit) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(textureUnit);
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

int setUpVao() {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VBO);
	fillVbo();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return vao;
}

void drawVao(unsigned int VAO, int numVerts) {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "4D Maze", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
	}
	BrickShader shader = BrickShader(camera);
	shader.use();

	stbi_set_flip_vertically_on_load(true);
	loadBrickTexture(GL_TEXTURE0);

	unsigned int VAO = setUpVao();

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, shader);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.updateUniforms();
		drawVao(VAO, numVerts);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}