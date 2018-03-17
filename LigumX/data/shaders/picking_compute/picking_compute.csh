#version 430

layout(local_size_x = 1, local_size_y = 1) in;

uniform sampler2D g_PickingBuffer;
uniform sampler2D g_DepthBuffer;

layout(std430, binding = 0) writeonly buffer g_OutputBuffer 
{
	vec4 outMousePosition;
};

float srgbToLinear(float S)
{
	return pow((S + 0.055f) / 1.055f, 2.4f);
}


uniform vec2 g_WindowSize;
uniform vec2 g_MousePosition;

void main() 
{
	vec2 coords = g_MousePosition / g_WindowSize;

	outMousePosition.rgba = texture(g_PickingBuffer, coords).rgba;

	//float depth = texture(g_DepthBuffer, coords).r;

	//outMousePosition.g = depth;
}