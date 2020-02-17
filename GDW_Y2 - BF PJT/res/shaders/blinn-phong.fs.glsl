#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec3 inTexWeights; // multiple textures with varying opacities

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform vec3  a_CameraPos;

#define MAX_LIGHTS 16
uniform int a_LightCount; // total amount of lights
uniform vec3 a_AmbientColor[MAX_LIGHTS];
uniform float a_AmbientPower[MAX_LIGHTS];

uniform vec3 a_LightPos[MAX_LIGHTS];
uniform vec3 a_LightColor[MAX_LIGHTS];
uniform float a_LightSpecPower[MAX_LIGHTS];
uniform float a_LightShininess[MAX_LIGHTS];
uniform float a_LightAttenuation[MAX_LIGHTS];

uniform sampler2D s_Albedos[3]; // now takes more than one value.

void main() {
    vec4 results[MAX_LIGHTS];

    vec4 finalResult = vec4(0, 0, 0, 0);

    // calculating the total amount of lights being used.
    int lightsTotal = (a_LightCount > MAX_LIGHTS) ? MAX_LIGHTS : (a_LightCount <= 0) ? 1 : a_LightCount;

    for(int i = 0; i < lightsTotal; i++)
    {
        // Re-normalize our input, so that it is always length 1
        vec3 norm = normalize(inNormal);

        // Determine the direction from the position to the light
        vec3 toLight = a_LightPos[i] - inWorldPos;
        
        // Determine the distance to the light (used for attenuation later)
        float distToLight = length(toLight);
        
        // Normalize our toLight vector
        toLight = normalize(toLight);

        // Determine the direction between the camera and the pixel
        vec3 viewDir = normalize(a_CameraPos - inWorldPos);

        // Calculate the halfway vector between the direction to the light and the direction to the eye
        vec3 halfDir = normalize(toLight + viewDir);

        // Our specular power is the angle between the the normal and the half vector, raised
        // to the power of the light's shininess
        float specPower = pow(max(dot(norm, halfDir), 0.0), a_LightShininess[i]);

        // Finally, we can calculate the actual specular factor
        vec3 specOut = specPower * a_LightColor[i];

        // Calculate our diffuse factor, this is essentially the angle between
        // the surface and the light
        float diffuseFactor = max(dot(norm, toLight), 0);

        // Calculate our diffuse output
        vec3 diffuseOut = diffuseFactor * a_LightColor[i];

        // Our ambient is simply the color times the ambient power
        vec3 ambientOut = a_AmbientColor[i] * a_AmbientPower[i];

        // We will use a modified form of distance squared attenuation, which will avoid divide
        // by zero errors and allow us to control the light's attenuation via a uniform
        float attenuation = 1.0 / (1.0 + a_LightAttenuation[i] * pow(distToLight, 2));
    
        float totalWeight = dot(inTexWeights, vec3(1, 1, 1));
	    vec3 weights = inTexWeights / totalWeight;
	    
        // Below is modified for tutorial 10
	    // Previously was: vec4 albedo = texture(s_Albedo, inUV);
	    vec4 albedo =
	    texture(s_Albedos[0], inUV) * weights.x +
	    texture(s_Albedos[1], inUV) * weights.y +
	    texture(s_Albedos[2], inUV) * weights.z;
	
	    // Our result is our lighting multiplied by our object's color
	    vec3 result = (ambientOut + attenuation * (diffuseOut + specOut)) * albedo.xyz * inColor.xyz;
        results[i] = vec4(result, inColor.a * albedo.a);

	    // TODO: gamma correction
	}

    // averaging the lights
    for(int i = 0; i < lightsTotal; i++)
    {
        finalResult += results[i];
    }

    finalResult.x = finalResult.x / lightsTotal;
    finalResult.y = finalResult.y / lightsTotal;
    finalResult.z = finalResult.z / lightsTotal;
    finalResult.w = finalResult.w / lightsTotal;
    
    finalResult.w = finalResult.w * a_Alpha;

	// Write the output
	outColor = finalResult;
}