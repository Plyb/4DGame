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

Camera camera;

const std::string RES_PATH = "./";
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

glm::mat4 createDropDimensionMat() {
	float testMat4[16] = {
		1.0f, 0.0f, 0.0f,					0.0f,
		0.0f, 1.0f, 0.0f,					0.0f,
		0.0f, 0.0f, cos(glm::radians(camera.psi)), 0.0f,
		0.0f, 0.0f, sin(glm::radians(camera.psi)), 0.0f
	};
	return glm::make_mat4(testMat4);
}

;
void fillVBO() {
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

	camera.yaw += xoffset;
	camera.pitch += yoffset;

	if (camera.pitch > 89.0f) {
		camera.pitch = 89.0f;
	}
	else if (camera.pitch < -89.0f) {
		camera.pitch = -89.0f;
	}
}

void processInput(GLFWwindow* window, const Shader& shader) {
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		proportion += 0.005;
		shader.setFloat("proportion", proportion);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		proportion -= 0.005;
		shader.setFloat("proportion", proportion);
	}

	glm::vec4 cameraFront = glm::vec4(
		cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)),
		sin(glm::radians(camera.pitch)),
		sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.psi)),
		sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.psi))
	);
	glm::vec4 cameraSide = glm::normalize(glm::vec4(
		-sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)),
		0.0f,
		cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.psi)),
		cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.psi))
	));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.pos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.pos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.pos -= cameraSide * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.pos += cameraSide * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.psi += 0.1f;
		fillVBO();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.psi -= 0.1f;
		fillVBO();
	}
	std::cout << (std::string) Vec4(camera.pos) << "   " << camera.psi << '\n';
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

glm::mat4 createModelTransform(glm::vec3 position, float rotationAngle, bool rotateOverTime) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	return glm::rotate(model, glm::radians(rotationAngle) + (float)glfwGetTime() * glm::radians(50.0f) * rotateOverTime, glm::vec3(1.0f, 0.3f, 0.5f));
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

int setUpVao() {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VBO);
	fillVBO();

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
	Shader shader = Shader((RES_PATH + "shader.vert").c_str(), (RES_PATH + "shader.frag").c_str());
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
		shader.setMatrix("view", createViewTransform());
		shader.setMatrix("projection", createProjectionTransform());
		shader.setMatrix("dropDimension", createDropDimensionMat());
		shader.setFloat("cameraPos", camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.w);
		drawVao(VAO, numVerts);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}