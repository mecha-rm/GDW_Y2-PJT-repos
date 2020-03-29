// gaussian blur (5 X 5)

#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

// matrix
//		   [ 1, 4, 6, 4, 1]
//		   [ 4, 16, 24, 16, 1]
// 1/256 * [ 6, 24, 36, 24, 6]
//		   [ 4, 16, 24, 16, 1]
//		   [ 1, 4, 6, 4, 1]
const float KERNEL[5][5] = 
{
	{1.0F / 256.0F * 1.0F, 1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 6.0F, 1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 1.0F},
	{1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 16.0F, 1.0F / 256.0F * 24.0F, 1.0F / 256.0F * 16.0F, 1.0F / 256.0F * 1.0F},
	{1.0F / 256.0F * 6.0F, 1.0F / 256.0F * 24.0F, 1.0F / 256.0F * 36.0F, 1.0F / 256.0F * 24.0F, 1.0F / 256.0F * 6.0F},
	{1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 16.0F, 1.0F / 256.0F * 24.0F, 1.0F / 256.0F * 16.0F, 1.0F / 256.0F * 1.0F},
	{1.0F / 256.0F * 1.0F, 1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 6.0F, 1.0F / 256.0F * 4.0F, 1.0F / 256.0F * 1.0F},
};

// applies the blur to the array of pixels
float ApplyBlur(float pixels[5][5])
{
	return (
		// row 0
		KERNEL[0][0] * pixels[0][0] +
		KERNEL[0][1] * pixels[0][1] +
		KERNEL[0][2] * pixels[0][2] +
		KERNEL[0][3] * pixels[0][3] +
		KERNEL[0][4] * pixels[0][4] +

		// row 1
		KERNEL[1][0] * pixels[1][0] +
		KERNEL[1][1] * pixels[1][1] +
		KERNEL[1][2] * pixels[1][2] +
		KERNEL[1][3] * pixels[1][3] +
		KERNEL[1][4] * pixels[1][4] +

		// row 2
		KERNEL[2][0] * pixels[2][0] +
		KERNEL[2][1] * pixels[2][1] +
		KERNEL[2][2] * pixels[2][2] +
		KERNEL[2][3] * pixels[2][3] +
		KERNEL[2][4] * pixels[2][4] +

		// row 3
		KERNEL[3][0] * pixels[3][0] +
		KERNEL[3][1] * pixels[3][1] +
		KERNEL[3][2] * pixels[3][2] +
		KERNEL[3][3] * pixels[3][3] +
		KERNEL[3][4] * pixels[3][4] +

		// row 4
		KERNEL[4][0] * pixels[4][0] +
		KERNEL[4][1] * pixels[4][1] +
		KERNEL[4][2] * pixels[4][2] +
		KERNEL[4][3] * pixels[4][3] +
		KERNEL[4][4] * pixels[4][4]
	);
}

// applies the blur to a series of values
float ApplyBlur(
	float r00, float r01, float r02, float r03, float r04,
	float r10, float r11, float r12, float r13, float r14,
	float r20, float r21, float r22, float r23, float r24,
	float r30, float r31, float r32, float r33, float r34,
	float r40, float r41, float r42, float r43, float r44)
{
	return (
		// row 0
		KERNEL[0][0] * r00 +
		KERNEL[0][1] * r01 +
		KERNEL[0][2] * r02 +
		KERNEL[0][3] * r03 +
		KERNEL[0][4] * r04 +

		// row 1
		KERNEL[1][0] * r10 +
		KERNEL[1][1] * r11 +
		KERNEL[1][2] * r12 +
		KERNEL[1][3] * r13 +
		KERNEL[1][4] * r14 +

		// row 2
		KERNEL[2][0] * r20 +
		KERNEL[2][1] * r21 +
		KERNEL[2][2] * r22 +
		KERNEL[2][3] * r23 +
		KERNEL[2][4] * r24 +

		// row 3
		KERNEL[3][0] * r30 +
		KERNEL[3][1] * r31 +
		KERNEL[3][2] * r32 +
		KERNEL[3][3] * r33 +
		KERNEL[3][4] * r34 +

		// row 4
		KERNEL[4][0] * r40 +
		KERNEL[4][1] * r41 +
		KERNEL[4][2] * r42 +
		KERNEL[4][3] * r43 +
		KERNEL[4][4] * r44
	);
}

// main
void main() {
	vec4 newColor = inColor; // the new color
	vec4 pxlArr[5][5]; // array of surrounding pixels
	float clrArr[5][5];

	vec2 pixelPos; // pixel position
	pixelPos.x = round(inScreenRes.x * inUV.x); 
	pixelPos.y = round(inScreenRes.y * inUV.y); 

	
	// getting the pixels
	// pixel row
	for(int row = -2; row <= 2; row++)
	{
		// pixel column
		for(int col = -2; col <= 2; col++)
		{
			// if within the bounds
			if(pixelPos.x + row >= 0 && pixelPos.x + row <= inScreenRes.x && pixelPos.y + col >= 0 && pixelPos.y + col <= inScreenRes.y)
			{
				pxlArr[2 + row][2 + col] = texture(xImage, vec2((pixelPos.x + row) / inScreenRes.x, (pixelPos.y + col) / inScreenRes.y));
			}
		}
	}


	// red
	newColor.r = ApplyBlur(
		pxlArr[0][0].r, pxlArr[0][1].r, pxlArr[0][2].r, pxlArr[0][3].r, pxlArr[0][4].r,
		pxlArr[1][0].r, pxlArr[1][1].r, pxlArr[1][2].r, pxlArr[1][3].r, pxlArr[1][4].r,
		pxlArr[2][0].r, pxlArr[2][1].r, pxlArr[2][2].r, pxlArr[2][3].r, pxlArr[2][4].r,
		pxlArr[3][0].r, pxlArr[3][1].r, pxlArr[3][2].r, pxlArr[3][3].r, pxlArr[3][4].r,
		pxlArr[4][0].r, pxlArr[4][1].r, pxlArr[4][2].r, pxlArr[4][3].r, pxlArr[4][4].r
	);

	// green
	newColor.g = ApplyBlur(
		pxlArr[0][0].g, pxlArr[0][1].g, pxlArr[0][2].g, pxlArr[0][3].g, pxlArr[0][4].g,
		pxlArr[1][0].g, pxlArr[1][1].g, pxlArr[1][2].g, pxlArr[1][3].g, pxlArr[1][4].g,
		pxlArr[2][0].g, pxlArr[2][1].g, pxlArr[2][2].g, pxlArr[2][3].g, pxlArr[2][4].g,
		pxlArr[3][0].g, pxlArr[3][1].g, pxlArr[3][2].g, pxlArr[3][3].g, pxlArr[3][4].g,
		pxlArr[4][0].g, pxlArr[4][1].g, pxlArr[4][2].g, pxlArr[4][3].g, pxlArr[4][4].g
	);

	// blue
	newColor.b = ApplyBlur(
		pxlArr[0][0].b, pxlArr[0][1].b, pxlArr[0][2].b, pxlArr[0][3].b, pxlArr[0][4].b,
		pxlArr[1][0].b, pxlArr[1][1].b, pxlArr[1][2].b, pxlArr[1][3].b, pxlArr[1][4].b,
		pxlArr[2][0].b, pxlArr[2][1].b, pxlArr[2][2].b, pxlArr[2][3].b, pxlArr[2][4].b,
		pxlArr[3][0].b, pxlArr[3][1].b, pxlArr[3][2].b, pxlArr[3][3].b, pxlArr[3][4].b,
		pxlArr[4][0].b, pxlArr[4][1].b, pxlArr[4][2].b, pxlArr[4][3].b, pxlArr[4][4].b
	);

	outColor = vec4(newColor.rgb, inColor.a);
}