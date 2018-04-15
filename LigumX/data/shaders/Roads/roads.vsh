#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 vpMat;
uniform mat4 g_ModelToWorldMatrix;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_ProjectionMatrix;
uniform mat4 g_LightProjectionMatrix;
// Include ProvidersMarker

out vec2 myTexCoord;


out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() {
	vec4 vWorldPosition = g_ModelToWorldMatrix * vec4(pos, 1);
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vWorldPosition;

    myTexCoord = texCoord;

}
