#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

// the resolution of the image, which is the same as xBufferSize in post.vs.glsl.
uniform vec2 xBufferSize;

// the kernel for the operation
uniform mat3 a_Kernel;

float KernelFilter(mat3 pixels)
{
	return (
		// row 1
		a_Kernel[0][0] * pixels[0][0] +
		a_Kernel[0][1] * pixels[0][1] +
		a_Kernel[0][2] * pixels[0][2] +

		// row 2
		a_Kernel[1][0] * pixels[1][0] +
		a_Kernel[1][1] * pixels[1][1] +
		a_Kernel[1][2] * pixels[1][2] +

		// row 3
		a_Kernel[2][0] * pixels[2][0] +
		a_Kernel[2][1] * pixels[2][1] +
		a_Kernel[2][2] * pixels[2][2]
	);
}

void main() {
	vec4 newColor = inColor; // the new color
	vec4 pxlArr[3][3]; // array of surrounding pixels
	mat3 clrArr; // matrix of surroundin colours

	vec2 pixelPos; // pixel position
	pixelPos.x = round(xBufferSize.x * inUV.x); 
	pixelPos.y = round(xBufferSize.y * inUV.y); 

	
	// getting the pixels
	// top row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xBufferSize.x, (pixelPos.y + 1) / xBufferSize.y)); // left 1, up 1
//	pxArr[0][1] = texture(xImage, vec2((pixelPos.x) / xBufferSize.x, (pixelPos.y + 1) / xBufferSize.y)); // up 1
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xBufferSize.x, (pixelPos.y + 1) / xBufferSize.y)); // right 1, up 1
//
//	// middle row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xBufferSize.x, (pixelPos.y) / xBufferSize.y)); // left 1
//	pxArr[0][1] = texture(xImage, inUV); // centre
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xBufferSize.x, (pixelPos.y) / xBufferSize.y)); // right 1
//
//	// bottom row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xBufferSize.x, (pixelPos.y - 1) / xBufferSize.y)); // left 1, down 1
//	pxArr[0][1] = texture(xImage, vec2((pixelPos.x) / xBufferSize.x, (pixelPos.y - 1) / xBufferSize.y)); // down 1
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xBufferSize.x, (pixelPos.y - 1) / xBufferSize.y)); // right 1, down 1
	
	// pixel row
	for(int row = -1; row <= 1; row++)
	{
		// pixel column
		for(int col = -1; col <= 1; col++)
		{
			// if within the bounds
			if(pixelPos.x + row >= 0 && pixelPos.x + row <= xBufferSize.x && pixelPos.y + col >= 0 && pixelPos.y + col <= xBufferSize.y)
			{
				pxlArr[1 + row][1 + col] = texture(xImage, vec2((pixelPos.x + row) / xBufferSize.x, (pixelPos.y + col) / xBufferSize.y));
			}
		}
	}
	
	// red
	newColor.r = KernelFilter(mat3(
		pxlArr[0][0].r, pxlArr[0][1].r, pxlArr[0][2].r,
		pxlArr[1][0].r, pxlArr[1][1].r, pxlArr[1][2].r,
		pxlArr[2][0].r, pxlArr[2][1].r, pxlArr[2][2].r
		)
	);

	// green
	newColor.g = KernelFilter(mat3(
		pxlArr[0][0].g, pxlArr[0][1].g, pxlArr[0][2].g,
		pxlArr[1][0].g, pxlArr[1][1].g, pxlArr[1][2].g,
		pxlArr[2][0].g, pxlArr[2][1].g, pxlArr[2][2].g
		)
	);

	// blue
	newColor.b = KernelFilter(mat3(
		pxlArr[0][0].b, pxlArr[0][1].b, pxlArr[0][2].b,
		pxlArr[1][0].b, pxlArr[1][1].b, pxlArr[1][2].b,
		pxlArr[2][0].b, pxlArr[2][1].b, pxlArr[2][2].b
		)
	);

	outColor = vec4(newColor.rgb, inColor.a);
}