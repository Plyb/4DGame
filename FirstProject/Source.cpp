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

const std::string RES_PATH = "./";
const int vertexSize = 7;
float proportion = 0.2;
glm::vec4 cameraPos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float psi = 30.0f;
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
		0.0f, 0.0f, cos(glm::radians(psi)), 0.0f,
		0.0f, 0.0f, sin(glm::radians(psi)), 0.0f
	};
	return glm::make_mat4(testMat4);
}

;
void fillVBO() {
	/*Tetrahedron tetra(
		Vec4(0.0f, 0.0f, 0.0f, 1.0f),
		Vec4(-1.0f, 0.0f, 0.0f, -1.0f),
		Vec4(1.0f, 0.0f, 0.0f, -1.0f),
		Vec4(0.0f, 1.0f, 0.0f, -1.0f)
	);*//*
	std::vector<Tetrahedron> tetras = Cube(Vec4(-0.5f, -0.5f, -0.5f, 0.0f), 3).getTetrahedra();*/
	std::vector<Tetrahedron> tetras = Tesseract(Vec4(-0.5f, -0.5f, -0.5f, -0.5f)).getTetrahedra();
	Hyperplane hp(Vec4(0.0f, 0.0f, sin(glm::radians(psi)), -cos(glm::radians(psi))), Vec4(cameraPos));
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

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
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
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * cos(glm::radians(psi)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * sin(glm::radians(psi))
	);
	glm::vec4 cameraSide = glm::normalize(glm::vec4(
		-sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
		0.0f,
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * cos(glm::radians(psi)),
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * sin(glm::radians(psi))
	));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= cameraSide * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += cameraSide * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		psi += 0.1f;
		cameraPos = glm::vec4(0.0f, 0.0f, 2* cos(glm::radians(psi)), 2*  sin(glm::radians(psi)));
		fillVBO();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		psi -= 1.0f;
	}
	std::cout << (std::string) Vec4(cameraPos) << "   " << psi << '\n';
}

int createTexture(const char* location, int textureUnit, int sourceType) {
	int width, height, depth, nrChannels;
	unsigned char* data = stbi_load((RES_PATH + location).c_str(), &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_3D, texture);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		BrickTexture texture;
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, texture.sideLength, texture.sideLength, texture.sideLength, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels);
		glGenerateMipmap(GL_TEXTURE_3D);
	}
	else {
		std::cout << "Failed to load Texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_3D, texture);
	return texture;
}

glm::mat4 createModelTransform(glm::vec3 position, float rotationAngle, bool rotateOverTime) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	return glm::rotate(model, glm::radians(rotationAngle) + (float)glfwGetTime() * glm::radians(50.0f) * rotateOverTime, glm::vec3(1.0f, 0.3f, 0.5f));
}

glm::mat4 createViewTransform() {
	glm::vec3 threeCameraPos(cameraPos.x, cameraPos.y, cos(glm::radians(psi)) * cameraPos.z + sin(glm::radians(psi)) * cameraPos.w);
	glm::vec3 cameraFront(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	);
	glm::mat4 view = glm::lookAt(threeCameraPos, threeCameraPos + cameraFront, cameraUp);
	return view;
}

glm::mat4 createProjectionTransform() {
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

// UNUSED
glm::mat4 createTransform() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.1f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	return trans;
}

int setUpVAO() {

	//int x = 4;
	//glm::vec4 vert(vertexVector[0 + x], vertexVector[1 + x], vertexVector[2 + x], vertexVector[3 + x]);
	//glm::vec4 centered = vert - cameraPos;
	//glm::vec4 dimDropped = createDropDimensionMat() * centered;
	//glm::vec4 test = (dimDropped +
	//		glm::vec4(cameraPos.x, cameraPos.y, cos(glm::radians(psi)) + sin(glm::radians(psi)), 1.0f));


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	fillVBO();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

// UNUSED
int setUpDoubleTriangleVAO() {
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f
	};
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

// UNUSED
int setUpSingleTriangleVAO(const float* vertices) {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

// UNUSED
void cycleColor(const Shader& shader) {
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	shader.setFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
}

// UNUSED
void renderVAOwithEBO(unsigned int VAO) {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// UNUSED
void renderVAOwithVBO(unsigned int VAO, int numVerts) {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
}

void renderVAOsWithVBO(unsigned int VAO, int numVerts, Shader& shader) {
	/*glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/

	glBindVertexArray(VAO);
	//glm::mat4 model = createModelTransform(glm::vec3(), 0.0f, false);
	//shader.setMatrix("model", model);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	/*for (unsigned int i = 0; i < 10; i++) {
		glm::mat4 model = createModelTransform(cubePositions[i], 20.0f * i, i % 3 == 0);
		shader.setMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
	}*/
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
	int containerTexture = createTexture("container.jpg", GL_TEXTURE0, GL_RGB);
	//int faceTexture = createTexture("awesomeface.png", GL_TEXTURE1, GL_RGBA);
	//shader.setInt("tex", 0);
	//shader.setInt("texture2", 1);

	unsigned int VAO = setUpVAO();

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
		shader.setFloat("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.w);
		renderVAOsWithVBO(VAO, numVerts, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}