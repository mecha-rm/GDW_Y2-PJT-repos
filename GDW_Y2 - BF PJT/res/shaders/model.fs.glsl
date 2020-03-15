#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform vec3 a_CameraPos;

uniform vec3 a_Weights; // the weights of the entity.

uniform sampler2D s_Albedos[3]; // now takes more than one value.

void main() {
	vec3 weights = a_Weights;

	if(weights == vec3(0, 0, 0))
		weights = vec3(1.0F / 3.0F);
	
	// texture
    vec4 albedo =
        texture(s_Albedos[0], inUV) * weights.x +
	    texture(s_Albedos[1], inUV) * weights.y +
	    texture(s_Albedos[2], inUV) * weights.z;
    
    vec3 result = albedo.rgb * inColor.rgb;

	// Write the output
	outColor = vec4(result, inColor.a * albedo.a * a_Alpha);
}