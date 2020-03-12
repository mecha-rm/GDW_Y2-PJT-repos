#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

// the kernel for the operation (no mat5 exists)
uniform float a_Kernel[5][5];

// array version
float KernelFilter(float pixels[5][5])
{
	return (
		// row 0
		a_Kernel[0][0] * pixels[0][0] +
		a_Kernel[0][1] * pixels[0][1] +
		a_Kernel[0][2] * pixels[0][2] +
		a_Kernel[0][3] * pixels[0][3] +
		a_Kernel[0][4] * pixels[0][4] +

		// row 1
		a_Kernel[1][0] * pixels[1][0] +
		a_Kernel[1][1] * pixels[1][1] +
		a_Kernel[1][2] * pixels[1][2] +
		a_Kernel[1][3] * pixels[1][3] +
		a_Kernel[1][4] * pixels[1][4] +

		// row 2
		a_Kernel[2][0] * pixels[2][0] +
		a_Kernel[2][1] * pixels[2][1] +
		a_Kernel[2][2] * pixels[2][2] +
		a_Kernel[2][3] * pixels[2][3] +
		a_Kernel[2][4] * pixels[2][4] +

		// row 3
		a_Kernel[3][0] * pixels[3][0] +
		a_Kernel[3][1] * pixels[3][1] +
		a_Kernel[3][2] * pixels[3][2] +
		a_Kernel[3][3] * pixels[3][3] +
		a_Kernel[3][4] * pixels[3][4] +

		// row 4
		a_Kernel[4][0] * pixels[4][0] +
		a_Kernel[4][1] * pixels[4][1] +
		a_Kernel[4][2] * pixels[4][2] +
		a_Kernel[4][3] * pixels[4][3] +
		a_Kernel[4][4] * pixels[4][4]
	);
}

// indivdiaul value version
float KernelFilter(
	float r00, float r01, float r02, float r03, float r04,
	float r10, float r11, float r12, float r13, float r14,
	float r20, float r21, float r22, float r23, float r24,
	float r30, float r31, float r32, float r33, float r34,
	float r40, float r41, float r42, float r43, float r44)
{
	return (
		// row 0
		a_Kernel[0][0] * r00 +
		a_Kernel[0][1] * r01 +
		a_Kernel[0][2] * r02 +
		a_Kernel[0][3] * r03 +
		a_Kernel[0][4] * r04 +

		// row 1
		a_Kernel[1][0] * r10 +
		a_Kernel[1][1] * r11 +
		a_Kernel[1][2] * r12 +
		a_Kernel[1][3] * r13 +
		a_Kernel[1][4] * r14 +

		// row 2
		a_Kernel[2][0] * r20 +
		a_Kernel[2][1] * r21 +
		a_Kernel[2][2] * r22 +
		a_Kernel[2][3] * r23 +
		a_Kernel[2][4] * r24 +

		// row 3
		a_Kernel[3][0] * r30 +
		a_Kernel[3][1] * r31 +
		a_Kernel[3][2] * r32 +
		a_Kernel[3][3] * r33 +
		a_Kernel[3][4] * r34 +

		// row 4
		a_Kernel[4][0] * r40 +
		a_Kernel[4][1] * r41 +
		a_Kernel[4][2] * r42 +
		a_Kernel[4][3] * r43 +
		a_Kernel[4][4] * r44
	);
}

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
	newColor.r = KernelFilter(
		pxlArr[0][0].r, pxlArr[0][1].r, pxlArr[0][2].r, pxlArr[0][3].r, pxlArr[0][4].r,
		pxlArr[1][0].r, pxlArr[1][1].r, pxlArr[1][2].r, pxlArr[1][3].r, pxlArr[1][4].r,
		pxlArr[2][0].r, pxlArr[2][1].r, pxlArr[2][2].r, pxlArr[2][3].r, pxlArr[2][4].r,
		pxlArr[3][0].r, pxlArr[3][1].r, pxlArr[3][2].r, pxlArr[3][3].r, pxlArr[3][4].r,
		pxlArr[4][0].r, pxlArr[4][1].r, pxlArr[4][2].r, pxlArr[4][3].r, pxlArr[4][4].r
	);

	// green
	newColor.g = KernelFilter(
		pxlArr[0][0].g, pxlArr[0][1].g, pxlArr[0][2].g, pxlArr[0][3].g, pxlArr[0][4].g,
		pxlArr[1][0].g, pxlArr[1][1].g, pxlArr[1][2].g, pxlArr[1][3].g, pxlArr[1][4].g,
		pxlArr[2][0].g, pxlArr[2][1].g, pxlArr[2][2].g, pxlArr[2][3].g, pxlArr[2][4].g,
		pxlArr[3][0].g, pxlArr[3][1].g, pxlArr[3][2].g, pxlArr[3][3].g, pxlArr[3][4].g,
		pxlArr[4][0].g, pxlArr[4][1].g, pxlArr[4][2].g, pxlArr[4][3].g, pxlArr[4][4].g
	);

	// blue
	newColor.b = KernelFilter(
		pxlArr[0][0].b, pxlArr[0][1].b, pxlArr[0][2].b, pxlArr[0][3].b, pxlArr[0][4].b,
		pxlArr[1][0].b, pxlArr[1][1].b, pxlArr[1][2].b, pxlArr[1][3].b, pxlArr[1][4].b,
		pxlArr[2][0].b, pxlArr[2][1].b, pxlArr[2][2].b, pxlArr[2][3].b, pxlArr[2][4].b,
		pxlArr[3][0].b, pxlArr[3][1].b, pxlArr[3][2].b, pxlArr[3][3].b, pxlArr[3][4].b,
		pxlArr[4][0].b, pxlArr[4][1].b, pxlArr[4][2].b, pxlArr[4][3].b, pxlArr[4][4].b
	);

	outColor = vec4(newColor.rgb, inColor.a);
}