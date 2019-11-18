#version 450
layout (location = 0) in vec3 inTexCoords;
layout (location = 0) out vec4 outFragColor;

uniform samplerCube s_Skybox;

void main() {
	outFragColor = texture(s_Skybox, normalize(inTexCoords).xzy);
}