// Terrain Renderer (Vertex Shader)
#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outWorldPos;
layout (location = 3) out vec2 outUV;
layout (location = 4) out vec3 outTexWeights;
layout (location = 5) out float outVertHeight; // vertex height from the height map

uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;

uniform sampler2D a_TextureSampler; // height map
uniform float a_HeightMin; // minimum height
uniform float a_HeightMax; // maximum height

void main() {
	outColor = inColor;
	outNormal = a_NormalMatrix * inNormal;
	outWorldPos = (a_Model * vec4(inPosition, 1)).xyz;

	vec3 vertPos = inPosition;


	// height map
	// since the colour map is black and white, the r, g, and b values will all be the same. So it doesn't matter which one we choose.
	// white goes up, black stays the same

	float t = texture(a_TextureSampler, inUV).r; // treats the pixel colour as the value of 't'
	vertPos.z = (1.0f - t) * a_HeightMin + t * a_HeightMax; // lerps between the maximum and minimum height.

	outVertHeight =  vertPos.z; // saving the position of the vertex

	gl_Position = a_ModelViewProjection * vec4(vertPos, 1);
	
	// you'll need to change this based on the current terrain.
	outTexWeights = vec3(
		sin(inPosition.x / 2.0f) / 2 + 0.5,
		cos(inPosition.x * 3.7f) / 2 + 0.5,
		sin(inPosition.y) / 2 + 0.5
	);


	// using the point in world space as our UV coordinates. When fed into the shader, it will know that it's between [0, 1]
	// the position in world will be used as the UV coordinates
	outUV = vertPos.xy;
}
	