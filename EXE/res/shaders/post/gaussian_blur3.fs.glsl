// gaussian blur (3 X 3)
#version 440
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

// matrix
//		  [ 1, 2, 1]
// 1/16 * [ 2, 4, 2]
//		  [ 1, 2, 1]
const mat3 KERNEL = mat3
(
	1.0F / 16.0F * 1.0F, 1.0F / 16.0F * 2.0F, 1.0F / 16.0F * 1.0F,
	1.0F / 16.0F * 2.0F, 1.0F / 16.0F * 4.0F, 1.0F / 16.0F * 2.0F,
	1.0F / 16.0F * 1.0F, 1.0F / 16.0F * 2.0F, 1.0F / 16.0F * 1.0F
);

// applies the blur
float ApplyBlur(mat3 pixels)
{
	// matrix
	//		  [ 1, 2, 1]
	// 1/16 * [ 2, 4, 2]
	//		  [ 1, 2, 1]
	return (
		// row 0
		KERNEL[0][0] * pixels[0][0] +
		KERNEL[0][1] * pixels[0][1] +
		KERNEL[0][2] * pixels[0][2] +

		// row 1
		KERNEL[1][0] * pixels[1][0] +
		KERNEL[1][1] * pixels[1][1] +
		KERNEL[1][2] * pixels[1][2] +

		// row 2
		KERNEL[2][0] * pixels[2][0] +
		KERNEL[2][1] * pixels[2][1] +
		KERNEL[2][2] * pixels[2][2]
	);
}

// main
void main() {
	vec4 newColor = inColor; // the new color
	vec4 pxlArr[3][3]; // array of surrounding pixels

	vec2 pixelPos; // pixel position
	pixelPos.x = round(inScreenRes.x * inUV.x); 
	pixelPos.y = round(inScreenRes.y * inUV.y); 

	
	// getting the pixels

	// pixel row
	for(int row = -1; row <= 1; row++)
	{
		// pixel column
		for(int col = -1; col <= 1; col++)
		{
			// if within the bounds
			if(pixelPos.x + row >= 0 && pixelPos.x + row <= inScreenRes.x && pixelPos.y + col >= 0 && pixelPos.y + col <= inScreenRes.y)
			{
				pxlArr[1 + row][1 + col] = texture(xImage, vec2((pixelPos.x + row) / inScreenRes.x, (pixelPos.y + col) / inScreenRes.y));
			}
		}
	}
	
	// red
	newColor.r = ApplyBlur(mat3(
		pxlArr[0][0].r, pxlArr[0][1].r, pxlArr[0][2].r,
		pxlArr[1][0].r, pxlArr[1][1].r, pxlArr[1][2].r,
		pxlArr[2][0].r, pxlArr[2][1].r, pxlArr[2][2].r
		)
	);

	// green
	newColor.g = ApplyBlur(mat3(
		pxlArr[0][0].g, pxlArr[0][1].g, pxlArr[0][2].g,
		pxlArr[1][0].g, pxlArr[1][1].g, pxlArr[1][2].g,
		pxlArr[2][0].g, pxlArr[2][1].g, pxlArr[2][2].g
		)
	);

	// blue
	newColor.b = ApplyBlur(mat3(
		pxlArr[0][0].b, pxlArr[0][1].b, pxlArr[0][2].b,
		pxlArr[1][0].b, pxlArr[1][1].b, pxlArr[1][2].b,
		pxlArr[2][0].b, pxlArr[2][1].b, pxlArr[2][2].b
		)
	);

	outColor = vec4(newColor.rgb, inColor.a);
}