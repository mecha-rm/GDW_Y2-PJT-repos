// a shader for a sepia tone filter
#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

void main() {
	// sepia tone maximum colour. RGB = (112, 66, 20)
	vec3 sepiaColor = vec3(0.43921568627F, 0.25882352941F, 0.07843137254F);

	// the pixel colour
	vec4 imgClr = texture(xImage, inUV);
	
	// result
	// a percentage of the sepia tone colour is calculated based on the original rgb values in reference to the white value of (1, 1, 1);
	vec3 result = vec3(sepiaColor.r * imgClr.r, sepiaColor.g * imgClr.g, sepiaColor.b * imgClr.b);

	outColor = vec4(result, inColor.a * imgClr.a);
}