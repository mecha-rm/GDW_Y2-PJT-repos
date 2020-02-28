#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outScreenCoords; // instead of the world position, it's the screen coordinates
layout (location = 3) out vec2 outUV;

// uniform mat4 a_ModelViewProjection;
// uniform mat4 a_Model;
// uniform mat4 a_ModelView;
// uniform mat3 a_NormalMatrix;

// screen resolution
uniform vec2 xScreenRes;

// the aspect ratio of the frame
// uniform vec2 xAspectRatio;

void main() {
	outColor = inColor;
	outNormal = inNormal;
	outScreenCoords = ((inPosition.xy + vec2(1, 1)) * 2.0f) * xScreenRes;
	outUV = inUV;

	// vec3 newPos = inPosition;
	// newPos.xy *= xAspectRatio;
	// 
	// gl_Position = vec4(newPos, 1);
	gl_Position = vec4(inPosition, 1);
}