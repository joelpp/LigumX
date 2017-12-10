#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

uniform vec3 g_Color;

layout (location = 0) out vec4 FinalColor;


void main() 
{
	float threshold = 0.99f;
	float alpha = 1.f;
	if (myTexCoord.x < threshold && 
		myTexCoord.x > 1.f - threshold && 
		myTexCoord.y < threshold && 
		myTexCoord.y > 1.f - threshold)
	{
		alpha = 0.f;
	}


	FinalColor = vec4(g_Color, alpha);
	//vec3 fNormalWS = normalize(vNormalWS);
	//vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
	


}