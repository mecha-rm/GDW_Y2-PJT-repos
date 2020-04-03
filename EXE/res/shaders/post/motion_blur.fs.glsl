#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;
layout (binding = 1) uniform sampler2D a_CurrentDepth;

uniform sampler2D xImage;

uniform mat4 a_ViewProjectionInv;
uniform mat4 a_PrevViewProjection;

// const int c_NumSamples = 5;
// number of samples
uniform int a_Samples; // modifable (changed form tutorial)


// https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect
vec2 CalculateMotion(sampler2D depth, mat4 invViewProj, mat4 prevViewProj) {
	// Get the depth buffer value at this pixel.    
	float zOverW = texture(depth, inUV).r; 
	// H is the viewport position at this pixel in the range -1 to 1.    
	vec4 currentPos = vec4(inUV.x * 2 - 1, (1 - inUV.y) * 2 - 1, zOverW, 1); 
	// Transform by the view-projection inverse.    
	vec4 D = currentPos * invViewProj; 
	// Divide by w to get the world position.    
	vec4 worldPos = D / D.w;

	// We can determine this fragment's approximate position last frame
	// by multiplying by the last frame's view projection
	vec4 previousPos = worldPos * prevViewProj; 
	previousPos /= previousPos.w; 
	return (currentPos - previousPos).xy / 2.0f;
}

// calculates the blur
vec4 CalculateBlur(sampler2D sampler, vec2 motion, vec2 texCoord, const int samples) {
	// Get the initial color at this pixel.    
	vec4 color = texture(sampler, texCoord); 
	texCoord += motion; 
	for(int i = 1; i < samples; ++i, texCoord += motion) {   
		// Sample the color buffer along the velocity vector.    
		vec4 currentColor = texture(sampler, texCoord);   
		// Add the current color to our color sum.   
		color += currentColor; 
	} 
	// Average all of the samples to get the final blur color.    
	vec4 finalColor = color / samples;
	return finalColor;
}

void main() {
	// calculates the motion
	vec2 motion = CalculateMotion(a_CurrentDepth, a_ViewProjectionInv, a_PrevViewProjection);

	// there are samples to choose form
	if(a_Samples > 0)
		outColor = CalculateBlur(xImage, motion, inUV, a_Samples);
	else
		outColor = texture(xImage, inUV);
}