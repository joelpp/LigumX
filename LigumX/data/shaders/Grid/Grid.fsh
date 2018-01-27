#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosLightSpace;

#define PROVIDER_View
#define PROVIDER_Window

// Include Providers Marker

layout (location = 0) out vec4 FinalColor;

vec3 GetWorldSpaceRay(vec3 ndc)
{
	vec4 clipCoords = vec4(ndc.xy, -1.0f, 1.0f);

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


void main() 
{
	vec2 normalizedScreenPosition = gl_FragCoord.xy / g_WindowSize;

	vec3 ndc = vec3(2.f * normalizedScreenPosition - vec2(1.f, 1.f), 1.f);

	vec3 worldSpaceRay = GetWorldSpaceRay(ndc);

	if (worldSpaceRay.z > 0.f)
	{
		discard;
	}

	vec3 wsPosition = GetAimingWorldSpacePosition(worldSpaceRay);

	int g_Extent = 200;

	// from http://madebyevan.com/shaders/grid/
	// Compute anti-aliased world-space grid lines
	vec2 grid = abs(fract( (wsPosition.xy / g_Extent) - 0.5) - 0.5) / fwidth(wsPosition.xy / g_Extent);
	float line = min(grid.x, grid.y);

	// Just visualize the grid lines directly
	float alpha = 1.0 - min(line, 1.0);

	vec3 outputColor = vec3(1,1,1);

	clamp(outputColor, vec3(0, 0, 0), vec3(1, 1, 1));

	FinalColor = vec4(outputColor, alpha);

}