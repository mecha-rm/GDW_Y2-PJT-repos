// blinn-phong post processing
#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth; // Camera's depth buffer
layout(binding = 2) uniform sampler2D s_GNormal;     // The normal buffer

uniform sampler2D xImage;

// The inverse of the camera's view-project matrix (clip->world)
uniform mat4 a_ViewProjectionInv;

// The position of the camera, in world space
uniform vec3 a_CameraPos;

#define MAX_LIGHTS 20

// struct for lights
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

// enabled lights
uniform int a_EnabledLights;

// lights
uniform Light a_Lights[MAX_LIGHTS];

// if 'true', then background lighting is ignored.
uniform int a_IgnoreBackground;

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

// computes hte blinn-phong equation.
vec3 ComputeBlinnPhong(Light light, vec4 worldPosition, vec3 normal) {    

    // Determine the direction from the position to the light
    vec3 toLight = light.position - worldPosition.xyz;
    
    // Determine the distance to the light (used for attenuation later)
    float distToLight = length(toLight);
    
    // Normalize our toLight vector
    toLight = normalize(toLight);

    // Determine the direction between the camera and the pixel
    vec3 viewDir = normalize(a_CameraPos - worldPosition.xyz);

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
	return vec3(ambientOut + attenuation * (diffuseOut + specOut)) * inColor.xyz;
}


void main() {
	vec4 imgPx = texture(xImage, inUV);
	
    // extract the world position from the depth buffer
	vec4 worldPosition = GetWorldPosition(inUV);  

	// Extract our normal from the G Buffer
	vec3 worldNormal = UnpackNormal(texture(s_GNormal, inUV).rgb);

    // getting the enabled lights
    int enabledLights = clamp(a_EnabledLights, 0, MAX_LIGHTS); // clamping for minimum and maximum
	
    vec4 finalResult = vec4(0, 0, 0, 1);

    // background should use clear color
    if(a_IgnoreBackground > 0 && texture(s_CameraDepth, inUV).r * 2 - 1 == 1.0F)
	{
		finalResult.rgb = vec3(1, 1, 1); // no changes
	}
    else
    {
        for(int i = 0; i < enabledLights; i++)
        {
            finalResult.rgb += vec3(ComputeBlinnPhong(a_Lights[i], worldPosition, worldNormal));
        }

        // averaging the final result.
        finalResult.x /= enabledLights;
        finalResult.y /= enabledLights;
        finalResult.z /= enabledLights;
        finalResult.w = 1.0F;
    }

	outColor = vec4(finalResult.rgb * imgPx.rgb, inColor.a * imgPx.a);
}