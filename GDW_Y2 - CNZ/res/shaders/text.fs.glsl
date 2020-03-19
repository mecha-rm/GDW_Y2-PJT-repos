// reference: https://learnopengl.com/In-Practice/Text-Rendering

#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform vec3 a_CameraPos;

// text sampler
uniform sampler2D a_FontMap;

 // the color of the text.
uniform vec4 a_Color;

void main() {
	// source texture color.
    vec4 txtClr = texture(a_FontMap, inUV);
    vec3 result = inColor.rgb * (vec3(1, 1, 1) - txtClr.rgb) * a_Color.rgb;

	// Write the output
	outColor = vec4(result, inColor.a * txtClr.a * a_Color.a);
}