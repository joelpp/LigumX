#version 410 core

out vec4 color;

flat in int vertexID;

void main() 
{
    color = vec4(1.0);

	if (vertexID == -1)
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
}