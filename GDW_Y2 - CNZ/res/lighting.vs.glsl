#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outWorldPos;
layout (location = 3) out vec2 outUV;
layout (location = 4) out vec3 outTexWeights;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;

void main() {
	outColor = inColor;
	outNormal = a_NormalMatrix * inNormal;
	outColor = inColor;
	outWorldPos =  (a_Model * vec4(inPosition, 1)).xyz;
	gl_Position = a_ModelViewProjection * vec4(inPosition, 1);
	
	// you'll need to change this based on the current terrain.
	outTexWeights = vec3(
		sin(inPosition.x / 2.0f) / 2 + 0.5,
		cos(inPosition.x * 3.7f) / 2 + 0.5,
		sin(inPosition.y) / 2 + 0.5
	);


	outUV = inUV;
}