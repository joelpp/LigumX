#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

uniform vec3 g_Color;

layout (location = 0) out vec4 FinalColor;

float GetLineWidth(vec2 uv)
{
	vec2 grid = abs(fract( (uv) - 0.5) - 0.5) / fwidth(uv);
	float line = min(grid.x, grid.y);
	return line;
}

void main() 
{
	float threshold = GetLineWidth(myTexCoord);
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