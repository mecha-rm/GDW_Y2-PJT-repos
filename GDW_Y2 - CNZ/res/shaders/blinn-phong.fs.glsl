#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec3 inTexWeights; // multiple textures with varying opacities

layout (location = 0) out vec4 outColor;

uniform float a_Alpha; // the overall alpha value of the entity.

uniform vec3  a_CameraPos;

#define MAX_LIGHTS 20

// light struct
struct Light
{
	vec3 position;
	vec3 color;

	vec3 ambientColor;
	float ambientPower;

	float specularPower;
	float shininess;
	float attenuation;
};

uniform int a_EnabledLights; // total amount of lights

// lights
uniform Light a_Lights[MAX_LIGHTS];

uniform sampler2D s_Albedos[3]; // now takes more than one value.

// computes the blinn-phong equation.
vec3 ComputeBlinnPhong(Light light, vec3 normal, vec4 albedo) {    
    // Determine the direction from the position to the light
    vec3 toLight = light.position - inWorldPos;
    
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
    float specPower = pow(max(dot(normal, halfDir), 0.0), light.shininess);

    // Finally, we can calculate the actual specular factor
    vec3 specOut = specPower * light.color;

    // Calculate our diffuse factor, this is essentially the angle between
    // the surface and the light
    float diffuseFactor = max(dot(normal, toLight), 0);

    // Calculate our diffuse output
    vec3 diffuseOut = diffuseFactor * light.color;

    // Our ambient is simply the color times the ambient power
    vec3 ambientOut = light.ambientColor * light.ambientPower;

    // We will use a modified form of distance squared attenuation, which will avoid divide
    // by zero errors and allow us to control the light's attenuation via a uniform
    float attenuation = 1.0 / (1.0 + light.attenuation* pow(distToLight, 2));
	
	// Our result is our lighting multiplied by our object's color
	return vec3(ambientOut + attenuation * (diffuseOut + specOut)) * albedo.xyz * inColor.xyz;
}

void main() {
    vec4 results[MAX_LIGHTS];

    // re-normalize our input, so that it is always length 1 
    vec3 normal = normalize(inNormal);
    
    // weight calculation
    float totalWeight = dot(inTexWeights, vec3(1, 1, 1));
	vec3 weights = inTexWeights / totalWeight;

     // albedo for the entity.
     vec4 albedo =
        texture(s_Albedos[0], inUV) * weights.x +
	    texture(s_Albedos[1], inUV) * weights.y +
	    texture(s_Albedos[2], inUV) * weights.z;

    vec4 finalResult = vec4(0, 0, 0, 1);

    // calculating the total amount of lights being used.
    // int lightsTotal = (a_EnabledLights > MAX_LIGHTS) ? MAX_LIGHTS : (a_EnabledLights <= 0) ? 1 : a_EnabledLights;
    int enabledLights = clamp(a_EnabledLights, 0, MAX_LIGHTS);

    // getting all results
    for(int i = 1; i <= enabledLights; i++)
    {
        finalResult.rgb += ComputeBlinnPhong(a_Lights[i - 1], inNormal, albedo);
	}

    // averaging the final result.
    finalResult.x /= enabledLights;
    finalResult.y /= enabledLights;
    finalResult.z /= enabledLights;
    finalResult.w = finalResult.w * inColor.a * a_Alpha;

	// Write the output
	outColor = finalResult;
}