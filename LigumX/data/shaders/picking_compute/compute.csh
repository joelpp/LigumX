#version 430

layout(local_size_x = 1, local_size_y = 1) in;

uniform sampler2D g_PickingBuffer;

layout(std430, binding = 0) writeonly buffer g_OutputBuffer 
{
	vec3 outMousePosition;
};

uniform vec2 g_MousePosition;

void main() 
{
	vec2 coords = g_MousePosition / vec2(800, 800);
	coords.y = 1.f - coords.y;

	outMousePosition = texture(g_PickingBuffer, coords).rgb;

}