#version 430 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

#define PROVIDER_View
#define PROVIDER_Window
#define PROVIDER_WorldGrid
#define PROVIDER_Depth
#define PROVIDER_DataInspector

//layout(std430, binding = 7) writeonly buffer g_DataInspectorLayout
//{
//	float g_InspectorData[];
//};

// Include ProvidersMarker

layout (location = 0) out vec4 FinalColor;

vec3 GetWorldSpaceRay(vec3 ndc)
{
	vec4 clipCoords = vec4(ndc.xyz, 1.0f);

	vec4 cameraRay = g_ProjectionMatrixInverse * clipCoords;

	cameraRay.z = -1;
	cameraRay.w = 0;

	vec4 worldSpaceRay = g_ViewMatrixInverse * cameraRay;

	return normalize(worldSpaceRay.xyz);
}

vec3 GetAimingWorldSpacePosition(vec3 worldSpaceRay)
{
	vec3 cameraPosition = g_CameraPosition;

	vec3 planeNormal = vec3(0, 0, 1);
	vec3 pointOnPlane = vec3(0, 0, 0);

	float t = dot(pointOnPlane - cameraPosition, planeNormal) / dot(worldSpaceRay.xyz, planeNormal);

	vec3 worldPosition = cameraPosition + t * worldSpaceRay.xyz;

	return worldPosition;
}

float GetLineWidth(vec2 wsPosition, float gridExtent)
{
	vec2 grid = abs(fract( (wsPosition.xy / gridExtent) - 0.5) - 0.5) / fwidth(wsPosition.xy / gridExtent);
	float line = min(grid.x, grid.y);
	return line;
}

float GetLineAlpha(float lineWidth, float maximum)
{
	return maximum - min(lineWidth, maximum);
}


void main() 
{
	vec2 normalizedScreenPosition = gl_FragCoord.xy / g_WindowSize;
	vec2 normalizedMousePosition = vec2(g_MouseX, g_MouseY) / g_WindowSize;


	//float closestDepth = texture(g_DepthTexture, myTexCoord.xy).r;
	float closestDepth = texelFetch(g_DepthTexture, ivec2(gl_FragCoord.xy), 0).r;
	if ((g_MouseX == int(gl_FragCoord.x)) && (g_MouseY == int(gl_FragCoord.y)))
	{
		g_InspectorData[5] = closestDepth;
	}

	if (closestDepth < 1.f)
	{
		discard;
	}
	vec3 ndc = vec3(2.f * normalizedScreenPosition - vec2(1.f, 1.f), closestDepth);

	vec3 worldSpaceRay = GetWorldSpaceRay(ndc);

	if (worldSpaceRay.z > 0.f)
	{
		discard;
	}

	vec3 wsPosition = GetAimingWorldSpacePosition(worldSpaceRay);

	// from http://madebyevan.com/shaders/grid/
	float line = GetLineWidth(wsPosition.xy, g_WorldScale);
	float alpha = GetLineAlpha(line, 0.8f);

	vec3 outputColor = g_SectorGridColor;

	if ((abs(wsPosition.x) < 1.f) || (abs(wsPosition.y) < 1.f))
	{
		outputColor.gb *= 0.3f;
	}

	clamp(outputColor, vec3(0, 0, 0), vec3(1, 1, 1));

	FinalColor = vec4(outputColor, alpha);
	//FinalColor = vec4((wsPosition.z < 0.f) ? 0.f : 1.f, 0, 0, 1);
}