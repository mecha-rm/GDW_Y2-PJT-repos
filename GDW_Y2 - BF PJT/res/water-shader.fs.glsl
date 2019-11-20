#version 410
layout(location = 0) in vec4 inColor;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inWorldPos;
layout(location = 0) out vec4 outColor;

uniform vec3 a_CameraPos; // used for reflections; can be used to effect water opacity as well.

uniform vec3 a_WaterColor; // The color of the water
uniform float a_WaterAlpha; // The alpha value for all water rendering (quick hack for transparent water)
uniform float a_WaterClarity; // Mixing value for water albedo and reflection / refraction effects
uniform float a_FresnelPower; // How much reflection is applied
uniform float a_RefractionIndex; // Should be source / material refractive index (1 / 1.33 for water)

uniform samplerCube s_Environment;

void main() {
	// Re-normalize our input, so that it is always length 1
	vec3 norm = normalize(inNormal);
	
	// Determine the direction between the camera and the pixel
	vec3 viewDir = normalize(inWorldPos - a_CameraPos);
	
	vec3 reflection = normalize(reflect(viewDir, norm));
	vec3 refraction = normalize(refract(viewDir, norm, a_RefractionIndex));
	
	vec3 reflected = texture(s_Environment, reflection.xzy).rgb; 
	vec3 refracted = texture(s_Environment, refraction.xzy).rgb; // refract one vector out, and another vector down.

	// Calculate our fresnel power
	vec3 fresnel = vec3(dot(-viewDir, norm)) * a_FresnelPower;
	
	// Combine our refracted and reflected components
	vec3 environmentVal = refracted * (1.0 - fresnel) + reflected * fresnel;
	
	// We mix together our albedo and our water's clarity
	vec3 result = mix(a_WaterColor, environmentVal, a_WaterClarity);
	
	// TODO: gamma correction
	// Write the output
	outColor = vec4(result, inColor.a * a_WaterAlpha);
}