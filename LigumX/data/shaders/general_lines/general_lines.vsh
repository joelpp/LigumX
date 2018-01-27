#version 410 core
layout(location=0) in vec3 pos;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_View

// Include ProvidersMarker

out VertexData
{
  vec3 m_WorldPosition;
} outData;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	vec4 vWorldPosition = g_ModelToWorldMatrix * vec4(pos, 1);

	outData.m_WorldPosition = vWorldPosition.xyz;

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vec4(pos, 1);
}