#version 410 core

layout (location = 0) out vec4 FinalColor;

uniform vec3 g_InputColor;

void main() 
{
	FinalColor = vec4(g_InputColor, 1.0f);
}