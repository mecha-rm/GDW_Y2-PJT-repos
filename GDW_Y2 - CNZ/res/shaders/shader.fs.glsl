#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // alpha value

uniform vec3 a_CameraPos; // camera position

// uniform vec4 a_ColorMultiplier;

// uniform sampler2D s_Albedo

void main() {
	outColor = vec4(inColor.rgb, inColor.a * a_Alpha); // * a_ColorMultiplier;
}