// TODO: complete linear blur
#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

// rendered image
uniform sampler2D xImage;

// length of the blur
uniform float a_Length;

// the angle of the linear blur
uniform float a_Angle;

// amount of samples taken for each side
// e.g. 5 samples with an angle of 0 would be 5 samples from the left and 5 from the right.
uniform int a_Samples;

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
vec4 ApplyBlur(vec4 pixel, float theta, float blurLength, int samples)
{
	// the end result
	vec4 result = pixel;

	// gets the pixel position and the distance to the pixel
	vec2 pixelPos = inUV * inScreenRes;

	// rotates the length by the rotation factor
	// centre of the rotated vector
	vec2 rotLenCenter = vec2(blurLength / 2.0F, 0);

	vec2 rotLen = Rotate(vec2(blurLength) - rotLenCenter, theta) ;

	// determines whether to restrict uvs or not.
	bool uvRestrict = (a_RestrictUVs > 0) ? true : false;

	// increment for the rotation length vector
	// vec2 rotLenInc = rotLen / samples * 2;
	// vec2 rotLenInc = vec2(blurLength / samples * 2, 0);
	vec2 rotLenInc = Rotate(vec2(blurLength, 0), theta) / samples * 2;

	// vec2 rotLenNorm = normalize(Rotate(vec2(blurLength, 0), theta));

	// the amount of values.
	int vals = 1;

	// applies all samples
	for(int i = 1; i <= samples; i++)
	{
		// gets the pixel to both sides of the current pixel
		vec2 mixPixelPosA = pixelPos + rotLenInc * i;
		vec2 mixPixelPosB = pixelPos + rotLenInc * -i;
		// vec2 mixPixelPosB = pixelPos + rotLenNorm * -i;
		// vec2 mixPixelPosA = pixelPos + rotLenNorm * i;

		// gets the mix pixels uvs
		vec2 mixPixelUvA = vec2(mixPixelPosA.x / inScreenRes.x, mixPixelPosA.y / inScreenRes.y);
		vec2 mixPixelUvB = vec2(mixPixelPosB.x / inScreenRes.x, mixPixelPosB.y / inScreenRes.y);

		// pixel A is in range
		if(!uvRestrict || (uvRestrict && mixPixelUvA == clamp(mixPixelUvA, 0.0F, 1.0F)))
		{
			// gets the pixel colour
			result += texture(xImage, mixPixelUvA);
			vals++;
		}
		
		// pixel B is in range
		if(!uvRestrict || (uvRestrict && mixPixelUvB == clamp(mixPixelUvB, 0.0F, 1.0F)))
		{
			// gets the pixel colour
			result += texture(xImage, mixPixelUvB);
			vals++;
		}
	}

	// averages the results
	result = result / vals;

	return result;
}

void main() {
	// getting the image pixel
	vec4 pixel = texture(xImage, inUV);

	// if there are more than 0 samples.
	if(a_Samples > 0)
		pixel = ApplyBlur(pixel, a_Angle, a_Length, a_Samples);
	

	// getting the out color
	outColor = pixel;
}