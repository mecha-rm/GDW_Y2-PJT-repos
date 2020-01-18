#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_ModelView;

void main() {
	outColor = inColor;
	outNormal = inNormal; //(a_ModelView * vec4(inNormal, 1)).xyz;
	outColor = inColor;
	gl_Position = a_ModelViewProjection * vec4(inPosition, 1);
}