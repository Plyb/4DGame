#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec4 vertexColor;
out vec3 TexCoord;

//uniform mat4 model;
uniform vec4 cameraPos;
uniform mat4 dropDimension;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * (dropDimension * (aPos - cameraPos) +
		vec4(cameraPos.xy, dropDimension[2][2] * cameraPos.z + dropDimension[3][2] * cameraPos.w, 1.0f));
	//vertexColor = vec4(aColor, 1.0f);
	TexCoord = aTexCoord;
}