// boots color vibrance
#version 440
layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

void main() {
	// color
	vec4 color = texture(xImage, inUV);
	
	// multiplier, which gets the brightest coloure
	float multi = 0;

	// gets the brightest colour
	multi = 1.0F / max(color.r, max(color.g, color.b));

	// increasing the vibrance
	color.r = clamp(color.r * multi, 0.0F, 1.0F);
	color.g = clamp(color.g * multi, 0.0F, 1.0F);
	color.b = clamp(color.b * multi, 0.0F, 1.0F);

	outColor = color;
}