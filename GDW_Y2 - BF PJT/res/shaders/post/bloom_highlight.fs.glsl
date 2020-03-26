#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

uniform float a_Threshold;

vec3 ExtractHighlight(vec3 color, float threshold)
{
	// // calculates the highlight and caps it
	vec3 temp = (color - vec3(threshold)) / (1 - threshold);
	temp = clamp(temp, 0.0F, 1.0F);
	
	return temp;

	// Determine our luminance, based on percieved brightness of colors
	// float luminance = dot(color, vec3(0.299, 0.587, 0.114)); 
	// // step will return 0 if luminance < threshhold, and 1 if otherwise
	// vec3 temp = color * step(a_Threshold, luminance); 
	// return temp;
}

void main() {
	vec4 color = texture(xImage, inUV);
	float threshold = clamp(a_Threshold, 0.0F, 1.0F);
	
	color.rgb = ExtractHighlight(color.rgb, threshold);

	outColor = color;
}