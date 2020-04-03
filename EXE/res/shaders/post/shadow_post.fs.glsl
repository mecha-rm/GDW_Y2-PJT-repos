// this is just taken from the Intermediate Computer Graphics tutorial.
#version 440

// TODO:
// calculate the distance between the light and the pixel
// cast the shadow until you reach the skybox
// calculate the shadow's strength.


layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth; // Camera's depth buffer
layout(binding = 2) uniform sampler2D s_ShadowDepth; // The light's shadow sampler
layout(binding = 3) uniform sampler2D s_GNormal;     // The normal buffer
layout(binding = 4) uniform sampler2D s_Projection;  // The projection to use

// The inverse of the camera's view matrix (view->world)
uniform mat4 a_ViewInv;
// The inverse of the camera's project matrix (clip->view)
uniform mat4 a_ProjectionInv;
// The inverse of the camera's view-project matrix (clip->world)
uniform mat4 a_ViewProjectionInv;

// The near and far clip planes for the camera
uniform float a_NearPlane;
uniform float a_FarPlane;

// A matrix going from the world to the light space (world->light) basically the inverse of the light's transform
uniform mat4  a_LightView;
// The light's position, in world space
uniform vec3  a_LightPos;
// The light's direction, in world space
uniform vec3  a_LightDir;
// The light's color
uniform vec3  a_LightColor;
// The attenuation factor for the light (1/dist)
uniform float a_LightAttenuation;
// The shadow biasing to use
uniform float a_Bias = 0.01;
// This should really be a GBuffer parameter
uniform float a_MatShininess;

// Allows us to toggle between shadows and projectors
uniform bool  b_IsProjector;
// The intensity of the projector image
uniform float a_ProjectorIntensity;

const vec3 HALF = vec3(0.5);
const vec3 DOUBLE = vec3(2.0);

// Unpacks a normal from the [0,1] range to the [-1, 1] range
vec3 UnpackNormal(vec3 rawNormal) {
	return (rawNormal - HALF) * DOUBLE;
}

// Calculates a world position from the main camera's depth buffer
vec4 GetWorldPos(vec2 uv) {
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

vec4 GetViewPos(vec2 uv) {
	// Get the depth buffer value at this pixel.    
	float zOverW = texture(s_CameraDepth, uv).r * 2 - 1;
	// H is the viewport position at this pixel in the range -1 to 1.    
	vec4 currentPos = vec4(uv.xy * 2 - 1, zOverW, 1);
	// Transform by the view-projection inverse.    
	vec4 D = a_ProjectionInv * currentPos;
	// Divide by w to get the world position.    
	vec4 viewPos = D / D.w;
	return viewPos;
}

// Caluclate the blinn-phong factor
vec3 BlinnPhong(vec3 fragPos, vec3 fragNorm, vec3 lightPosition, vec3 lightColor, float lAttenuation, float shadowFactor) {
	// Determine the direction from the position to the light
	vec3 toLight = lightPosition - fragPos;

	// Determine the distance to the light (used for attenuation later)
	float distToLight = length(toLight);
	// Normalize our toLight vector
	toLight = normalize(toLight);

	// Determine the direction between the camera and the pixel (camera is now at 0,0,0)
	vec3 viewDir = normalize(-fragPos);

	// Calculate the halfway vector between the direction to the light and the direction to the eye
	vec3 halfDir = normalize(toLight + viewDir);

	// Our specular power is the angle between the the normal and the half vector, raised
	// to the power of the light's shininess
	float specPower = pow(max(dot(fragNorm, halfDir), 0.0), a_MatShininess);

	// Finally, we can calculate the actual specular factor
	vec3 specOut = specPower * lightColor;

	// Calculate our diffuse factor, this is essentially the angle between
	// the surface and the light
	float diffuseFactor = max(dot(fragNorm, toLight), 0);
	// Calculate our diffuse output
	vec3  diffuseOut = diffuseFactor * lightColor;

	// We will use a modified form of distance squared attenuation, which will avoid divide
	// by zero errors and allow us to control the light's attenuation via a uniform
	float attenuation = 1.0 / (1.0 + lAttenuation * pow(distToLight, 2));

	return (1.0 - shadowFactor) * attenuation * (diffuseOut + specOut);
}

// This function will sample multiple points around our sample, and average the results
// This gives a slight blur to the edges of the shadows, and helps to soften them up
// @param fragPos The position in the shadow's normalized clip space to sample
// @param bias The shadow bias factor to use
float PCF(vec3 fragPos, float bias) {
	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(s_ShadowDepth, 0); // Determine the texel size of the shadow sampler

	// Iterate over a 3x3 area of texels around our sample location
	for(int x = -1; x <= 1; ++x) { 
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(s_ShadowDepth, fragPos.xy + vec2(x, y) * texelSize).r; // Sample the texture
			result += fragPos.z - bias > pcfDepth ? 1.0 : 0.0; // Perform the depth test, and add the result to the sum
		}    
	}
	result /= 9.0; // Average our sum
	return result;
}

void main() {
	vec4 viewPos = GetViewPos(inUV);         // Extract the world position from the depth buffer
	vec4 shadowPos = a_LightView * viewPos;  // Determine the position in light clip space
	shadowPos /= shadowPos.w;                // Perspective divide
	shadowPos = shadowPos * 0.5 + 0.5;       // Normalize from clip space to [0,1]

	// Extract our normal from the G Buffer
	vec3 viewNormal = UnpackNormal(texture(s_GNormal, inUV).rgb);

	// Determine our biasing factor, we have a higher bias the closer the surface is to being parallell
	float bias = max((a_Bias * 10) * (1.0 - dot(viewNormal, a_LightDir)), a_Bias);
	// Determine our shadow factor using PCF
	float shadow = PCF(shadowPos.xyz, bias);
	// If we are outside of the range of our shadow texture, we set shadow to zero
	if (shadowPos.x < 0 || shadowPos.x > 1 ||
		shadowPos.y < 0 || shadowPos.y > 1 ||
		shadowPos.z < 0 || shadowPos.z > 1) { 
		shadow = 0;
	}

	vec3 result = vec3(0.0);
	// If this light is a projector, we handle things a little differently
    if (b_IsProjector) {
		// If the point we're trying to shade is outside of the lights clip region, we will simply discard and do no lighting
		if (shadowPos.x < 0 || shadowPos.x > 1 ||
			shadowPos.y < 0 || shadowPos.y > 1 ||
			shadowPos.z < 0 || shadowPos.z > 1) {
			discard;
		}
		// We can think of our projection texture as a filter over our light, so we can multiply them
		vec3 color = texture(s_Projection, shadowPos.xy).rgb * a_LightColor;
		// We can do our blinn-phong model using the calculated light to be projected
		result = BlinnPhong(viewPos.xyz, viewNormal, a_LightPos, color, a_LightAttenuation, shadow);
	} else {
		// This is not a projector, just do the normal blinn-phong model using the lights color
		result = BlinnPhong(viewPos.xyz, viewNormal, a_LightPos, a_LightColor, a_LightAttenuation, shadow);
	}
	// Output the result
	outColor = vec4(result, 1.0);
}