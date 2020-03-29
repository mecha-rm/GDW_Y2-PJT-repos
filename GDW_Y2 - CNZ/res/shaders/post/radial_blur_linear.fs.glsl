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
float a_Length;

// the angle of the linear blur
uniform float a_Angle;

// amount of samples taken for each side
// e.g. 5 samples with an angle of 0 would be 5 samples from the left and 5 from the right.
uniform int a_Samples;

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
	vec2 rotLen = Rotate(vec2(blurLength), theta);
	// centre of the rotated vector
	vec2 rotLenCenter = rotLen / 2.0F;

	// incremnet for the rotation length vector
	vec2 rotLenInc = rotLen / samples * 2;
	int vals = 1;

	// applies all samples
	for(int i = 1; i <= samples; i++)
	{
		// gets the pixel to both sides of the current pixel
		vec2 mixPixelPosA = pixelPos + rotLenInc * i;
		vec2 mixPixelPosB = pixelPos + rotLenInc * -i;

		// gets the mix pixels uvs
		vec2 mixPixelUvA = vec2(mixPixelPosA.x / inScreenRes.x, mixPixelPosA.y / inScreenRes.y);
		vec2 mixPixelUvB = vec2(mixPixelPosB.x / inScreenRes.x, mixPixelPosB.y / inScreenRes.y);

		// boundary checking
		vec2 tempA = clamp(mixPixelUvA, 0.0F, 1.0F);
		vec2 tempB = clamp(mixPixelUvB, 0.0F, 1.0F);

		// pixel A is in range
		if(mixPixelUvA == tempA)
		{
			// gets the pixel colour
			result += texture(xImage, mixPixelUvA);
			vals++;
		}
		
		// pixel B is in range
		if(mixPixelUvB == tempB)
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
	// getting the image
	vec4 img = texture(xImage, inUV);

	// getting the out color
	outColor = inColor * img;
}