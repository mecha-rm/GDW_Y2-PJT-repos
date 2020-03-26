// applies a lookup table
#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage; // image

uniform samplerCube a_Lut; // lookup table
// uniform sampler3D a_Lut; // lookup table; this should be the one that gets used, but it didn't work.

void main() {
	vec4 color = texture(xImage, inUV);
	vec4 newColor = texture(a_Lut, vec3(color.r, color.b, color.b)); // getting the colour value from the lookup table.
	// vec4 newColor = texture3D(a_Lut, vec3(color.r, color.b, color.b)); // getting the colour value from the lookup table.

	outColor = vec4(newColor.rgb, color.a);
}