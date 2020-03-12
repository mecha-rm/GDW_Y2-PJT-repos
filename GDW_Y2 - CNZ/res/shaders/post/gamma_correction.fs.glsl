// Reference: https://learnopengl.com/Advanced-Lighting/Gamma-Correction

// boots color vibrance
#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

uniform float a_Gamma;

void main() {
	// color
	vec4 color = texture(xImage, inUV);

	float gamma = 2.20F;

	// correction amount
	if(a_Gamma != 0.0F)
		gamma = a_Gamma;

	// gamma correction
	color.rgb = pow(color.rgb, vec3(1.0F/gamma));

	outColor = color;
}