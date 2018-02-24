#version 410 core

struct WayData
{
	int type;
	int IndexInSector;
};

layout(location=0) in vec3 pos;
layout(location=3) in ivec2 g_WayData;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_View

// Include ProvidersMarker

out VertexData
{
	flat int m_WayType;
	flat int m_Index;
} outData;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vec3 worldPos = pos;

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vec4(worldPos, 1);

	outData.m_WayType = g_WayData.x;
	outData.m_Index = g_WayData.y;
}