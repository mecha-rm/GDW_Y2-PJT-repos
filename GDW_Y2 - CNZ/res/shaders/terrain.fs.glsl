#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec3 inTexWeights; // multiple textures with varying opacities
layout (location = 5) in float inVertHeight; // the local position of the vertex

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform sampler2D s_Albedos[3]; // now takes more than one value.

void main() { 
    float totalWeight = dot(inTexWeights, vec3(1, 1, 1));
	vec3 weights;
	
	// weights = inTexWeights / totalWeight; // default

	weights = vec3(
		clamp((-inVertHeight + 0.5f) * 4.0f, 0.0f, 1.0f),
		min(clamp((-inVertHeight + 0.75f) * 4.0f, 0.0f, 1.0f), clamp((inVertHeight - 0.25f) * 4.0f, 0.0f, 1.0f)),
		clamp((inVertHeight - 0.5f) * 4.0f, 0.0f, 1.0f)
	);


	// getting the albedo value.
	vec4 albedo =
	texture(s_Albedos[0], inUV) * weights.x +
	texture(s_Albedos[1], inUV) * weights.y +
	texture(s_Albedos[2], inUV) * weights.z;
	
	vec3 result = albedo.xyz * inColor.xyz; // taking out lighting
	
	// Write the output
	outColor = vec4(result, 1 * a_Alpha); // * a_ColorMultiplier;
}