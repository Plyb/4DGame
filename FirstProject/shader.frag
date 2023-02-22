#version 330 core
in vec4 vertexColor;
in vec3 TexCoord;

out vec4 FragColor;

uniform sampler3D tex;
uniform float proportion;

void main() {
	FragColor = texture(tex, TexCoord);
}