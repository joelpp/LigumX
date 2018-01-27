#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosLightSpace;

#define PROVIDER_View

// Include ProvidersMarker

layout (location = 0) out vec4 FinalColor;


void main() 
{
	FinalColor = vec4(1, 0, 0, 1);

	//vec3 fNormalWS = normalize(vNormalWS);
	//vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
	


}