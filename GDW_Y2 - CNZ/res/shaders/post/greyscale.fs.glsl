#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

void main() {
	vec4 imgPx = texture(xImage, inUV);
	float res = (imgPx.r + imgPx.g + imgPx.b) / 3.0F;

	outColor = vec4(res, res, res, inColor.a * imgPx.a);
}