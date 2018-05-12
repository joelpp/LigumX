#version 410 core
layout(location=0) in vec3 pos;
layout(location=3) in vec3 v_Color;

uniform mat4 vpMat;

#define PROVIDER_View

// Include ProvidersMarker

out gl_PerVertex 
{
    vec4 gl_Position;
    float gl_PointSize;
};

out vec3 v_VertexColor;

void main() 
{
	vec3 worldToCam = pos - g_CameraPosition;
	float l = length(worldToCam);

	float lNorm = min(l / 10.f, 1.f);
	float f = 1.f/* - lNorm*/;
	gl_PointSize = 5.0 * f;

    gl_Position = vpMat * vec4(pos, 1);

	v_VertexColor = v_Color;
}