#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

// uniform float a_Alpha; // alpha value

uniform vec3 a_CameraPos; // camera position

// uniform sampler2D s_Albedo

// rendered image
uniform sampler2D xImage;

void main() {
	// getting the image
	vec4 img = texture(xImage, inUV);

	// getting the out color
	outColor = inColor * img;
}