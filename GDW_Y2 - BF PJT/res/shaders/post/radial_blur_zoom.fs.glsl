#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

// rendered image
uniform sampler2D xImage;

// the centre of the blur
vec2 a_Center;

// the intensity of the blur
float a_Intensity;

// the amount of samples for the blur
int a_Samples;

// rotates the vector
vec2 Rotate(vec2 vec, float angle)
{
	return vec2(
		vec.x * (cos(angle)) - vec.y * (sin(angle)), 
		vec.x * (sin(angle)) + vec.y * (cos(angle))
	);
}

vec4 ApplyBlur(vec4 pixel, vec2 center, float intensity, int samples)
{
	// the end result
	vec4 result = pixel;

	// gets the pixel position and the distance to the pixel
	vec2 pixelPos = inUV * inScreenRes;
	vec2 direcVec = center - pixelPos;
	vec2 direcNorm = normalize(direcVec); // normalized direction

	// makes the angle positive 
	// if(direcAngle < 0.0F)
	// 	direcAngle += radians(360.0F);

	// gets the incrementer for intensity
	float intenseInc = intensity / samples;
	int vals = 1; // the amount of values mixed in.

	// applies all samples
	for(int i = 1; i <= samples; i++)
	{
		// mix pixel position (goes in opposite direction)
		vec2 mixPixelPos = -direcNorm * intenseInc;

		// mix pixel uv
		vec2 mixPixelUv = vec2(mixPixelPos.x / inScreenRes.x, mixPixelPos.y / inScreenRes.y);

		// boundary checking
		vec2 temp = clamp(mixPixelUv, 0.0F, 1.0F);

		// pixel A is in range
		if(mixPixelUv == temp)
		{
			// gets the pixel colour
			result += texture(xImage, mixPixelUv);
			vals++;
		}
	}

	// averages results
	result = result / vals;

	return result;
}

// main
void main() {
	// getting the image pixel
	vec4 pixel = texture(xImage, inUV);

	// calculates blur
	if(a_Intensity != 0 && a_Samples != 0)
		pixel = ApplyBlur(pixel, a_Center, a_Intensity, a_Samples);

	// getting the out color
	outColor = pixel;
}