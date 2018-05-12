#version 410 core

out vec4 color;

in vec3 v_VertexColor;

void main() 
{
	color = vec4(v_VertexColor, 1.0);
}