#version 410 core
layout(location=0) in vec3 pos;

uniform mat4 vpMat;
uniform mat4 g_ModelToWorldMatrix;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_ProjectionMatrix;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
	vec4 vWorldPosition = g_ModelToWorldMatrix * vec4(pos, 1);
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vec4(pos, 1);
}