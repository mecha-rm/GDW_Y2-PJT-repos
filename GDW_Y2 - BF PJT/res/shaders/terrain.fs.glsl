#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec3 inTexWeights; // multiple textures with varying opacities
layout (location = 5) in float inT; // time value
layout (location = 6) in float inVertHeight; // the local position of the vertex

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform sampler2D s_Albedos[3]; // now takes more than one value.

// TODO: the weights just makei t one solid colour. You need to facotr in the height of the thing.

uniform float a_Weights[3]; // sets the weight of the terrain.

void main() { 
    // float totalWeight = dot(inTexWeights, vec3(1, 1, 1));
	vec3 weights = vec3(a_Weights[0], a_Weights[1], a_Weights[2]);
	
	// weights = inTexWeights / totalWeight; // default

	// this should really be passed in by the user.
	// weights = vec3(
	// 	clamp((-inVertHeight + 0.5f) * 4.0f, 0.0f, 1.0f),
	// 	min(clamp((-inVertHeight + 0.75f) * 4.0f, 0.0f, 1.0f), clamp((inVertHeight - 0.25f) * 4.0f, 0.0f, 1.0f)),
	// 	clamp((inVertHeight - 0.5f) * 4.0f, 0.0f, 1.0f)
	// );

	// if there are no weights.
	// if(weights == vec3(0, 0, 0))
	// 	weights = vec3(0.3F, 0.3F, 0.4F);

	// gets the weights as a vector.
	// vec3 weights = vec3(a_Weights[0], a_Weights[1], a_Weights[2]);
	
	// if no weights were provided, the weights are even.
	if(weights == vec3(0, 0, 0))
		weights = vec3(0.3F, 0.3F, 0.4F);
	
	// total of starting weights.
	float totalWeights = weights.x + weights.y + weights.z;
	
	// if the amount of total weights exceeds 1, the weights are averaged.
	if(totalWeights > 1.0F)
	{
		weights.x /= totalWeights;
		weights.y /= totalWeights;
		weights.z /= totalWeights;
	}

	// albedo for result.
	vec4 albedo = vec4(0);

	// using lerp, it determines how much of the given textures to mix.
	if(inT <= weights.x) // texture 0 is solid.
	{
		weights = vec3(1, 0, 0);
		albedo = texture(s_Albedos[0], inUV);
	}
	else if(inT <= weights.x + weights.y) // mixing texture 0 and texture 1.
	{
		// inverse lerp to find new (t) value.
		// since this uses the sum of weight x and y, the (val_end - val_start) operation is already done.
		// (val_want - val_start) / (val_end - val_start)
		float t = (inT - weights.x) / (weights.y);
		
		// gets the new colour.
		albedo = mix(
			texture(s_Albedos[0], inUV),
			texture(s_Albedos[1], inUV),
			t
		);
	}
	else if(inT < weights.x + weights.y + weights.z) // mixing texture 2 and texture 3.
	{
		// new t value.
		float t = (inT - (weights.x + weights.y)) / (weights.z);
		
		// gets the new colour.
		albedo = mix(
			texture(s_Albedos[1], inUV),
			texture(s_Albedos[2], inUV),
			t
		);

	}
	else // getting the albedo value (shouldn't be reached)
	{
		// this is the usual way texture mixing works.
		albedo =
		texture(s_Albedos[0], inUV) * weights.x +
		texture(s_Albedos[1], inUV) * weights.y +
		texture(s_Albedos[2], inUV) * weights.z;
	}

	// calculating result (no lighting)
	vec3 result = albedo.xyz * inColor.xyz;
	
	// Write the output
	outColor = vec4(result, 1 * a_Alpha); // * a_ColorMultiplier;
}