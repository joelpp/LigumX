#version 410 core

in vec2 myTexCoord;

#define PROVIDER_View

// Include ProvidersMarker

layout (location = 0) out vec4 FinalColor;


void main() 
{
	FinalColor = vec4(1, 0, 0, 1);

	float middle = 0.5f;
	float offset = 0.02f;
	bool laneX = (myTexCoord.x > (middle - offset)) && (myTexCoord.x < (middle + offset));

	
	bool splitY = (int(myTexCoord.y * 5) % 2) == 0;

	vec3 gray = vec3(169,169,169) / 255.f;
	vec3 yellow = vec3(250, 210, 1) / 255.f;
	vec3 black = vec3(0, 0, 0);

	vec3 color = gray;


	if (laneX && splitY) 
	{
		color = yellow;
	}

	bool debug_border = false;
	if (debug_border )
	{
		float borderOffset = 0.01f;
		bool border = (myTexCoord.y < borderOffset) || (myTexCoord.y > 1.f - borderOffset);

		color = border ? black : color;
	}

	FinalColor = vec4(color, 1);
	//vec3 fNormalWS = normalize(vNormalWS);
	//vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
	


}