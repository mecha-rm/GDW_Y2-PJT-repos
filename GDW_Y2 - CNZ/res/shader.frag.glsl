#version 410

layout (location = 0) in vec4 inColor;

layout (location = 0) out vec4 outColor;

uniform vec4 a_ColorMultiplier;

void main() {
	outColor = inColor;// * a_ColorMultiplier;
}