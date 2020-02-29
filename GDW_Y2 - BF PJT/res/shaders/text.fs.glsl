// reference: https://learnopengl.com/In-Practice/Text-Rendering

#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform vec3 a_CameraPos;

uniform sampler2D a_Text; // text sampler

void main() {
	// texture
    vec4 textImg = texture(a_Text, inUV);
    vec3 result = textImg.rgb * inColor.rgb;

	// Write the output
	outColor = vec4(result, inColor.a * textImg.a * a_Alpha);
}