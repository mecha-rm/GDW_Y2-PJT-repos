// calculates a point shadow
// TODO:
// calculate the distance between the light and the pixel
// cast the shadow until you reach the skybox
// calculate the shadow's strength.
// TODO: you need the depth of the original image.

#version 440

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inScreenCoords;
layout (location = 3) in vec2 inUV;
layout (location = 4) in vec2 inScreenRes;

layout (location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth;

// rendered image; this one may not have the depth
uniform sampler2D xImage;

// starting image; this one definitely has the depth.
uniform sampler2D xImageOrig;

// the depth of the original image
uniform sampler2D xImageDepthOrig;

#define MAX_LIGHTS 20

// The inverse of the camera's view-project matrix (clip->world)
uniform mat4 a_ViewProjectionInv;

// The position of the camera, in world space
uniform vec3 a_CameraPos;

// struct for lights
struct Light
{
	vec3 position;
	vec3 color;

	vec3 ambientColor;
	float ambientPower;

	float specularPower;
	float shininess;
	float attenuation;
};

// enabled lights
uniform int a_EnabledLights;

// lights
uniform Light a_Lights[MAX_LIGHTS];

// if 'true', then background lighting is ignored.
uniform int a_IgnoreBackground;

// unpacks a normal from the [0,1] range to the [-1, 1] range
vec3 UnpackNormal(vec3 rawNormal) { return (rawNormal - 0.5F) * 2.0F; }

// calculates a world position from the main camera's depth buffer
vec4 GetWorldPosition(sampler2D depth, vec2 uv) {
	// Get the depth buffer value at this pixel.    
	float zOverW = texture(depth, uv).r * 2 - 1; 

	// H is the viewport position at this pixel in the range -1 to 1.    
	vec4 currentPos = vec4(uv.xy * 2 - 1, zOverW, 1); 
	
    // Transform by the view-projection inverse.    
	vec4 D = a_ViewProjectionInv * currentPos; 

	// Divide by w to get the world position.    
	vec4 worldPos = D / D.w;

	return worldPos;
}

// gets the screen position.
vec2 GetScreenPosition(vec2 uv)
{
	return round(uv * inScreenRes);
}

// gets the depth of the pixel
float GetDepth(sampler2D depth, vec2 uv)
{
	return texture(depth, uv).r * 2 - 1;
}

// calculates the shadows
vec4 ComputeShadow(Light light, vec4 pixel, vec2 uv)
{
	vec4 result = pixel;

	// pixel screen position
	vec2 pixelScreenPos = GetScreenPosition(uv);

	// pixel world position
	// vec4 pixelWorldPos = GetWorldPosition(s_CameraDepth, uv);
	vec4 pixelWorldPos = GetWorldPosition(xImageDepthOrig, uv);

	// gets the pixel depth.
	// float pixelDepth = GetDepth(xImageDepthOrig, uv);
	float pixelDepth = GetDepth(s_CameraDepth, uv);

	// if the background should be ignored, and the pixel depth is 1.0F (i.e. it's a background pixel)
	if(a_IgnoreBackground > 0 && pixelDepth == 1.0F)
		return pixel;
	
	 // Determine the direction from the position to the light
    vec3 toLight = light.position - pixelWorldPos.xyz;
    
    // Determine the distance to the light (used for attenuation later)
    float distToLight = length(toLight);
	
	// calculates the strength of the light on the object.
	// if this results in a 0 value, then the light doesn't touch the object.
    float lightIntensity = 1.0 / (1.0 + light.attenuation* pow(distToLight, 2));

	// the light cannot reach the pixel
	// if(lightIntensity == 0)
	// 	return pixel;

	// gets the direction towards the light
	vec3 dir = vec3(
		(toLight.x > 0.0F) ? 1.0F : (toLight.x < 0.0F) ? -1.0F : 0.0F,
		(toLight.y > 0.0F) ? 1.0F : (toLight.y < 0.0F) ? -1.0F : 0.0F,
		(toLight.z > 0.0F) ? 1.0F : (toLight.z < 0.0F) ? -1.0F : 0.0F 
	);

	// checks along the axis if a shadow is being casted.
	bool axisCheck = true;

	// offset
	float offset = 1.0F;

	// x-axis check
	while(axisCheck)
	{
		// gets the uv of the next pixel
		vec2 nextPixelUV = vec2(pixelScreenPos.x + offset * dir.x, pixelScreenPos.y) / inScreenRes;

		// pixel out of bounds.
		if(nextPixelUV != clamp(nextPixelUV, 0.0F, 1.0F))
		{
			axisCheck = false;
			continue;
		}

		// gets the world position of the next pixel
		// vec4 nextPixelWpos = GetWorldPosition(s_CameraDepth, nextPixelUV);
		vec4 nextPixelWpos = GetWorldPosition(xImageDepthOrig, nextPixelUV);

		// gets the depth of the pixel
		// float nextPixelDepth = GetDepth(xImageDepthOrig, nextPixelUV);
		float nextPixelDepth = GetDepth(s_CameraDepth, nextPixelUV);

		// if the depth is greater than that of the current pixel, it is farther away from the light source.
		// if it is farther from the light source, it cannot cast a shadow on the current pixel.
		
		if(nextPixelDepth < pixelDepth)
		// if(length(light.position - nextPixelWpos.xyz) < distToLight)
		{
			result.rgb = light.ambientColor * light.ambientPower;
			axisCheck = false; // done checking the axes
			continue;
		}

		
		// preparing the next pixel
		offset += 1.0F;
		
		// checking to make sure the next uvs are within the bounds of the image.
		// if they aren't, then the axis check is finished.
		// uv of next pixel
		vec2 temp_uv = vec2(pixelScreenPos.x + offset * dir.x, pixelScreenPos.y) / inScreenRes;
		
		if(temp_uv != clamp(temp_uv, 0.0F, 1.0F)) // pixel out of bounds.
			axisCheck = false;

		// world position of next pixel
		// vec4 temp_wpos = GetWorldPosition(s_CameraDepth, temp_uv);
		vec4 temp_wpos = GetWorldPosition(xImageDepthOrig, temp_uv);

		// if the next pixel is past the light source, and thus can't cast a shadow on the current pixel.
		if(abs(pixelWorldPos.x - temp_wpos.x) > abs(toLight.x)) 
			axisCheck = false;
	}

	// if the pixel colour has changed, then it has been shadowed.
	if(pixel != result)
		return result;

	// resetting values for next axis check.
	axisCheck = true;
	offset = 1.0F;

	// y-axis check
	while(axisCheck)
	{
		// gets the uv of the next pixel
		vec2 nextPixelUV = vec2(pixelScreenPos.x, pixelScreenPos.y + offset * dir.y) / inScreenRes;

		// pixel out of bounds.
		if(nextPixelUV != clamp(nextPixelUV, 0.0F, 1.0F))
		{
			axisCheck = false;
			continue;
		}

		// gets the world position of the next pixel
		// vec4 nextPixelWpos = GetWorldPosition(s_CameraDepth, nextPixelUV);
		vec4 nextPixelWpos = GetWorldPosition(xImageDepthOrig, nextPixelUV);

		// gets the depth of the pixel
		// float nextPixelDepth = GetDepth(xImageDepthOrig, nextPixelUV);
		float nextPixelDepth = GetDepth(s_CameraDepth, nextPixelUV);
		// maybe change this to check distance to light?
		// if the depth is greater than that of the current pixel, it is farther away from the light source.
		// if it is farther from the light source, it cannot cast a shadow on teh current pixel.
		
		if(nextPixelDepth < pixelDepth)
		// if(length(light.position - nextPixelWpos.xyz) < distToLight)
		{
			result.rgb = light.ambientColor * light.ambientPower;
			axisCheck = false; // done checking the axes
			continue;
		}

		
		// preparing the next pixel
		offset += 1.0F;
		
		// checking to make sure the next uvs are within the bounds of the image.
		// if they aren't, then the axis check is finished.
		// uv of next pixel
		vec2 temp_uv = vec2(pixelScreenPos.x, pixelScreenPos.y + offset * dir.y) / inScreenRes;
		
		if(temp_uv != clamp(temp_uv, 0.0F, 1.0F)) // pixel out of bounds.
			axisCheck = false;

		// world position of next pixel
		// vec4 temp_wpos = GetWorldPosition(s_CameraDepth, temp_uv);
		vec4 temp_wpos = GetWorldPosition(xImageDepthOrig, temp_uv);

		// if the next pixel is past the light source, and thus can't cast a shadow on the current pixel.
		if(abs(pixelWorldPos.y - temp_wpos.y) > abs(toLight.y)) 
			axisCheck = false;
	}

	// if the pixel colour has changed, then it has been shadowed.
	if(pixel != result)
		return result;

	return result;
}

// main function
void main() {
	// getting the image
	vec4 pixel = texture(xImage, inUV);
	
	// checks to see if it's not a background pixel. If it isn't a background pixel, then a shadow could be casted.
	// if(GetDepth(xImageDepthOrig, inUV) < 1.0F)
	if(GetDepth(s_CameraDepth, inUV) < 1.0F)
	{
		vec4 finalResult = vec4(0.0F);

		// getting the enabled lights
		int enabledLights = clamp(a_EnabledLights, 0, MAX_LIGHTS); // clamping for minimum and maximum
		
		// amount of values summed
		int vals = 0;

		// goes through each light
		for(int i = 0; i < enabledLights; i++)
		{
			vec4 result = pixel;
			result = ComputeShadow(a_Lights[i], pixel, inUV);

			// shadow casted.
			if(result != pixel)
			{
				finalResult += result; // sums results
				vals++;
			}
		}

		// if values were added, they are averaged.
		// if values were not added, the finalResult is set to white so no changes occur.
		if(vals > 0)
		{
			finalResult = finalResult / vals;
		}
		else
		{
			finalResult = vec4(1.0F);
		}

		outColor = pixel * finalResult;
	}
	else
	{
		// getting the out color
		outColor = pixel;
	}

}