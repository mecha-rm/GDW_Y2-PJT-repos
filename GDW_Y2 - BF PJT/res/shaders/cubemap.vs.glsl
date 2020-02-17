#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 0) out vec3 outTexCoords;

uniform mat4 a_View;
uniform mat4 a_Projection;

void main()
{
	outTexCoords = normalize(inPosition);
	vec4 outPos = a_Projection * a_View * vec4(inPosition, 1.0);
	gl_Position = outPos.xyww;
}
