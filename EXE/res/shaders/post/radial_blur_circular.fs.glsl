// radial blur
// this is based on the depth of field shader provided in the tutorial
#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth; // Camera's depth buffer
layout(binding = 2) uniform sampler2D s_GNormal;     // The normal buffer

// rendered image
uniform sampler2D xImage;

// the centre of the blur (screen space)
uniform vec2 a_Center;

// the angle (in radians) of the blur.
// the samples are taken based on this angle
uniform float a_Angle;

// the amount of samples for the blur
uniform int a_Samples;

// positive is clockwise, negative is counter clockwise
uniform int a_Direction;

// if true (> 0), the uvs are clamped into the (0, 1) range.
uniform int a_RestrictUVs;

// rotates the vector
vec2 Rotate(vec2 vec, float angle)
{
	return vec2(
		vec.x * (cos(angle)) - vec.y * (sin(angle)), 
		vec.x * (sin(angle)) + vec.y * (cos(angle))
	);
}


// applies a blur
// the pixel colour, its uv, the centre of the blur, angle, its direction (cw vs. counter-cw)
vec4 ApplyBlur(vec4 pixel, vec2 center, float theta, bool clockWise, int samples)
{
	// the end result
	vec4 result = pixel;

	// gets the pixel position and the distance to the pixel
	vec2 pixelPos = inUV * inScreenRes;
	vec2 centerPos = center * inScreenRes;

	// determines whether to restrict uvs or not.
	bool uvRestrict = (a_RestrictUVs > 0) ? true : false;
	
	// gets the rotation factor
	float rotFactor = theta / samples;
	float direc = (clockWise) ? 1.0F : -1.0F;
	int vals = 1; // the amount of colour values mixed in.

	// applies all samples
	for(int i = 1; i <= samples; i++)
	{
		float angle = rotFactor * i * direc;

		// gets the position of the pixel being mixed in.
		vec2 mixPixelPos = Rotate(pixelPos - centerPos, angle) + centerPos;

		// vec2 mixPixelPos = vec2(
		// 	pixelPos.x * (cos(angle)) - pixelPos.y * (sin(angle)), 
		// 	pixelPos.x * (sin(angle)) + pixelPos.y * (cos(angle))
		// );

		// the mix pixel uv
		vec2 mixPixelUv = vec2(mixPixelPos.x / inScreenRes.x, mixPixelPos.y / inScreenRes.y);

		// checks to see if the rotation got an 'out of bounds' pixel
		vec2 temp = mixPixelUv;

		// pixel in range. If uvRestrict is false, then pixels are used regardless of their uvs.
		// what happens in this case is determined by OpenGL.
		if(!uvRestrict || (uvRestrict && mixPixelUv == clamp(mixPixelUv, 0.0F, 1.0F)))
		{
			// gets the pixel colour
			result += texture(xImage, mixPixelUv);
			vals++;
		}
		else // pixel out of range
		{
			continue;
		}
	}

	// averages the results
	result = result / vals;

	return result;
}

void main() {
	// getting the image
	vec4 pixel = texture(xImage, inUV);

	if(a_Samples > 0)
	{
		int direc = clamp(a_Direction, -1, 1);

		// checks the rotation direction.
		switch(direc)
		{
			case 1: // clock-wise
				pixel = ApplyBlur(pixel, a_Center, a_Angle, true, a_Samples);
				break;

			case -1: // counter-clockwise
				pixel = ApplyBlur(pixel, a_Center, a_Angle, false, a_Samples);
				break;

			case 0: // clockwise and counter clockwise
			default:
				pixel = ApplyBlur(pixel, a_Center, a_Angle, true, a_Samples);
				pixel = ApplyBlur(pixel, a_Center, a_Angle, false, a_Samples);
				break;
		}
		
	}

	// getting the out color
	outColor = pixel;
}