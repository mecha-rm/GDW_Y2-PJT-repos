#version 410
#define M_PI 3.1415926535897932384626433832795

// Heavily inspired by:
// https://catlikecoding.com/unity/tutorials/flow/waves/
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

#define MAX_WAVES 8
uniform mat4 a_ModelViewProjection;
uniform mat4 a_ModelView;
uniform float a_Time;
uniform float a_Gravity; // This needs to match world units (ex: 9.81 if unit is meters)
uniform int a_EnabledWaves;
uniform vec4 a_Waves[MAX_WAVES];

vec3 GerstnerWave(vec4 waveInfo, vec3 pos, inout vec3 tangent, inout vec3 binorm) {
	// Our steepness is how 'sharp' the wave is
	float steepness = waveInfo.z;
	
	// The wave length is the size of the wave in meters
	float waveLength = waveInfo.w;
	
	// k is the frequency of the wave
	float k = 2 * M_PI / waveLength;
	
	// c is the speed of the wave, calculated from wave length and gravity (todo: move gravity to a uniform)
	// If we make gravity lower, the waves will move slower
	float c = sqrt(a_Gravity / k);
	
	// Extract the wave's direction (todo: normalize on the CPU side)
	vec2 dir = normalize(waveInfo.xy);
	
	// f is how far along in the direction the wave is at the given time
	float f = k * (dot(dir, pos.xy) - c * a_Time);
	
	// a is used to calculate our curve over our wavelength, and to prevent loops from forming
	float a = steepness / k;
	
	// Calculate our tangent and binormal factors; figures out which way the waves are pointing.
	tangent += vec3(
		(-dir.x * dir.x * (steepness * sin(f))),
		(-dir.x * dir.y * (steepness * sin(f))),
		( dir.x * (steepness * cos(f)))
	);

	binorm += vec3(
		(-dir.x * dir.y * (steepness * sin(f))),
		(-dir.y * dir.y * (steepness * sin(f))),
		( dir.y * (steepness * cos(f)))
	);

	// Calculate our position as affected by the wave
	return vec3(
		dir.x * (a * cos(f)),
		dir.y * (a * cos(f)),
		a * sin(f)
	);
}

void main() {
	outColor = inColor;
	vec3 pos = inPosition;
	
	vec3 tangent = vec3( 1, 0, 0);
	vec3 binorm = vec3( 0, 1, 0);
	
	vec3 result = pos;
	for (int ix = 0; ix < a_EnabledWaves && ix < MAX_WAVES; ix++) {
		result += GerstnerWave(a_Waves[ix], pos, tangent, binorm);
	}
	
	outNormal = normalize(cross(tangent, binorm));
	outWorldPos = result;
	gl_Position = a_ModelViewProjection * vec4(result, 1);
}