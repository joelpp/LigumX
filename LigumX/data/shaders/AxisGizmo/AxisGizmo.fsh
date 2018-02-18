#version 410 core

flat in int VertexID;


layout (location = 0) out vec4 FinalColor;

vec3 g_LineColors[6] = vec3[]
(
	vec3(1, 0, 0),
	vec3(1, 0, 0),
	vec3(0, 1, 0),
	vec3(0, 1, 0),
	vec3(0, 0, 1),
	vec3(0, 0, 1)
);

void main() 
{
	FinalColor.xyz = g_LineColors[VertexID];
	FinalColor.w = 1.f;
	
}