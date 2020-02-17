#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

// used for morph targets
layout (location = 4) in vec3 inPosition1;
layout (location = 5) in vec4 inColor1;
layout (location = 6) in vec3 inNormal1;
layout (location = 7) in vec2 inUV1;

layout (location = 8) in vec3 inPosition2;
layout (location = 9) in vec4 inColor2;
layout (location = 10) in vec3 inNormal2;
layout (location = 11) in vec2 inUV2;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outWorldPos;
layout (location = 3) out vec2 outUV;
layout (location = 4) out vec3 outTexWeights;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;

// do not use a_Time
uniform float a_T; // used to set the values for a morph target calculation.

void main() {
	

	// computing lerp ((1.0F - t) * v0 + t * v1) for morph targets
	vec3 newPos = (1.0F - a_T) * inPosition1 + a_T * inPosition2; // position
	vec4 newClr = (1.0F - a_T) * inColor1 + a_T * inColor2; // color
	vec3 newNorm = (1.0F - a_T) * inNormal1 + a_T * inNormal2; // normals 
	vec2 newUV = (1.0F - a_T) * inUV1 + a_T * inUV2; // uvs

	outNormal = a_NormalMatrix * newNorm;
	outWorldPos =  (a_Model * vec4(newPos, 1)).xyz;
	gl_Position = a_ModelViewProjection * vec4(newPos, 1);
	
	// you'll need to change this based on the current terrain.
	outTexWeights = vec3(
		sin(newPos.x / 2.0f) / 2 + 0.5,
		cos(newPos.x * 3.7f) / 2 + 0.5,
		sin(newPos.y) / 2 + 0.5
	);

	outColor = newClr;
	outUV = inUV;
}