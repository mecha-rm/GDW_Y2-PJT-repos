#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

// the overall alpha value of the entity.
uniform float a_Alpha; 

// camera position
uniform vec3 a_CameraPos;

// now takes more than one value.
uniform sampler2D s_Albedos[3]; 

// the weights of the entity.
uniform float a_Weights[3]; 

void main() {
	// gets the weights as a vector.
	vec3 weights = vec3(a_Weights[0], a_Weights[1], a_Weights[2]);

	// if no weights were provided, the weights are even.
	if(weights == vec3(0, 0, 0))
		weights = vec3(0.3F, 0.3F, 0.4F);

	// total of starting weights.
	float totalWeights = weights.x + weights.y + weights.z;

	// if the amount of total weights exceeds 1, the weights are averaged.
	if(totalWeights > 1)
	{
		weights.x /= totalWeights;
		weights.y /= totalWeights;
		weights.z /= totalWeights;
	}
	
	// texture
    vec4 albedo =
        texture(s_Albedos[0], inUV) * weights.x +
	    texture(s_Albedos[1], inUV) * weights.y +
	    texture(s_Albedos[2], inUV) * weights.z;
    
    vec3 result = albedo.rgb * inColor.rgb;

	// Write the output
	outColor = vec4(result, inColor.a * albedo.a * a_Alpha);
}