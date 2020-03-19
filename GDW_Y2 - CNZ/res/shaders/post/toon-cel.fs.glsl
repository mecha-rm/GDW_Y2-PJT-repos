// modeled after ICG_1 - LEC11
#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth; // Camera's depth buffer
layout(binding = 2) uniform sampler2D s_GNormal;     // The normal buffer

// rendered image
uniform sampler2D xImage;

// camera position
uniform vec3 a_CameraPos;

// The inverse of the camera's view-project matrix (clip->world)
uniform mat4 a_ViewProjectionInv;

// the size of the outline aorund the 
uniform float a_OutlineSize;

// the color of the outline (RGB)
uniform vec3 a_OutlineColor;

// the amount of levels for the bands
uniform int a_Levels;

// used for unpacking the normal
const vec3 HALF = vec3(0.5);
const vec3 DOUBLE = vec3(2.0);

// unpacks a normal from the [0,1] range to the [-1, 1] range
vec3 UnpackNormal(vec3 rawNormal) { return (rawNormal - HALF) * DOUBLE; }

// calculates a world position from the main camera's depth buffer
vec4 GetWorldPosition(vec2 uv) {
	// Get the depth buffer value at this pixel.    
	float zOverW = texture(s_CameraDepth, uv).r * 2 - 1; 

	// H is the viewport position at this pixel in the range -1 to 1.    
	vec4 currentPos = vec4(uv.xy * 2 - 1, zOverW, 1); 
	
    // Transform by the view-projection inverse.    
	vec4 D = a_ViewProjectionInv * currentPos; 

	// Divide by w to get the world position.    
	vec4 worldPos = D / D.w;

	return worldPos;
}

void main() {
	// getting the image
	vec4 color = texture(xImage, inUV);

	// getting the pixel's normal
	vec3 norm = texture(s_GNormal, inUV).rgb;

	// world position
	vec4 worldPos = GetWorldPosition(inUV);

	// levels (must be greater than 0)
	int levels = (a_Levels > 0) ? a_Levels : 1;
	

	// scale factor
	float scaleFactor = 1.0F / levels;

	// finding the pixel in reference to the camera. The closer the value is to 0, the closer it is to the edge.
	float edge = dot(normalize(a_CameraPos - worldPos.xyz), norm);

	// Write the output
	// edge = clamp(edge, 0, 1); // edge limits

	// size of outline
	// setting the colour for the edge, and writing output.
	if(edge < a_OutlineSize) // outline
	{
		color.rgb = a_OutlineColor; // black by default
	}
	else // no outline
	{
		color.rgb = floor(color.rgb * levels) * scaleFactor;
	}

	// color bounds
	color = clamp(color, 0, 1);

	// getting the out color
	outColor = color;
}