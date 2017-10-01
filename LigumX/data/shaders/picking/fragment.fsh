#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosLightSpace;


#define PROVIDER_Picking
// Include Providers Marker

out vec3 FinalColor;

void main() 
{
	FinalColor = vec3(g_PickingID, 1.f - g_PickingID, 0.f);
}