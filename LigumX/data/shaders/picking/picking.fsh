#version 410 core

in vec3 vWorldPosition;

#define PROVIDER_Picking
// Include Providers Marker

out vec4 FinalColor;

void main() 
{
	vec2 windowSize = vec2(128.f, 128.f) / 2.f;

	FinalColor = vec4(vWorldPosition.xyz, g_PickingID);
}