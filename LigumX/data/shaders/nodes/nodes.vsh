#version 410 core
layout(location=0) in vec3 pos;

uniform mat4 vpMat;

#define PROVIDER_View

// Include ProvidersMarker

out gl_PerVertex 
{
    vec4 gl_Position;
    float gl_PointSize;
};

flat out int vertexID;

void main() 
{
	vec3 worldToCam = pos - g_CameraPosition;
	float l = length(worldToCam);

	float lNorm = min(l / 10.f, 1.f);
	float f = 1.f/* - lNorm*/;
	gl_PointSize = 2.0 * f;

    gl_Position = vpMat * vec4(pos, 1);

	vertexID = gl_VertexID;
}