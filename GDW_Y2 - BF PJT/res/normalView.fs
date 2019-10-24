#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;

layout (location = 0) out vec4 outColor;

void main() {
	outColor = inColor;
	outColor.rgb = (inNormal.rgb / 2.0f) + vec3(0.5f);
}