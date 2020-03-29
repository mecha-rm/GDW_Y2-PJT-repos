// accumulates the frames and averages them
#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

// base image
uniform sampler2D xImage;

// starting image
uniform sampler2D xImageOrig;

// most recent image
uniform sampler2D xImageLast;

void main() 
{
	vec4 temp = texture(xImage, inUV) + texture(xImageOrig, inUV); 
	outColor = temp / 2.0F;	
}