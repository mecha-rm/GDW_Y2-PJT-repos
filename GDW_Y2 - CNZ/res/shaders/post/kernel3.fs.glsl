#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

// the resolution of the image, which is the same as xScreenRes in post.vs.glsl.
uniform vec2 xImageRes;

// the kernel for the operation
uniform mat3 xKernel;

float KernelFilter(mat3 pixels)
{
	return (
		// row 1
		xKernel[0][0] * pixels[0][0] +
		xKernel[0][1] * pixels[0][1] +
		xKernel[0][2] * pixels[0][2] +

		// row 2
		xKernel[1][0] * pixels[1][0] +
		xKernel[1][1] * pixels[1][1] +
		xKernel[1][2] * pixels[1][2] +

		// row 3
		xKernel[2][0] * pixels[2][0] +
		xKernel[2][1] * pixels[2][1] +
		xKernel[2][2] * pixels[2][2]
	);
}

void main() {
	vec4 newColor = inColor; // the new color
	vec4 pxlArr[3][3]; // array of surrounding pixels
	mat3 clrArr; // matrix of surroundin colours

	vec2 pixelPos; // pixel position
	pixelPos.x = round(xImageRes.x * inUV.x); 
	pixelPos.y = round(xImageRes.y * inUV.y); 

	
	// getting the pixels
	// top row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xImageRes.x, (pixelPos.y + 1) / xImageRes.y)); // left 1, up 1
//	pxArr[0][1] = texture(xImage, vec2((pixelPos.x) / xImageRes.x, (pixelPos.y + 1) / xImageRes.y)); // up 1
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xImageRes.x, (pixelPos.y + 1) / xImageRes.y)); // right 1, up 1
//
//	// middle row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xImageRes.x, (pixelPos.y) / xImageRes.y)); // left 1
//	pxArr[0][1] = texture(xImage, inUV); // centre
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xImageRes.x, (pixelPos.y) / xImageRes.y)); // right 1
//
//	// bottom row
//	pxArr[0][0] = texture(xImage, vec2((pixelPos.x - 1) / xImageRes.x, (pixelPos.y - 1) / xImageRes.y)); // left 1, down 1
//	pxArr[0][1] = texture(xImage, vec2((pixelPos.x) / xImageRes.x, (pixelPos.y - 1) / xImageRes.y)); // down 1
//	pxArr[0][2] = texture(xImage, vec2((pixelPos.x + 1) / xImageRes.x, (pixelPos.y - 1) / xImageRes.y)); // right 1, down 1
	
	// pixel row
	for(int row = -1; row <= 1; row++)
	{
		// pixel column
		for(int col = -1; col <= 1; col++)
		{
			// if within the bounds
			if(pixelPos.x + row >= 0 && pixelPos.x + row <= xImageRes.x && pixelPos.y + col >= 0 && pixelPos.y + col <= xImageRes.y)
			{
				pxlArr[1 + row][1 + col] = texture(xImage, vec2((pixelPos.x + row) / xImageRes.x, (pixelPos.y + col) / xImageRes.y));
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