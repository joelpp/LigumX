#version 410 core

#define PROVIDER_View

// Include ProvidersMarker

out gl_PerVertex {
    vec4 gl_Position;
};

flat out int VertexID;

const float g_AxisLength = 0.1f;
const vec4 g_ScreenOffset = vec4(0.85, -0.95, 0, 0);

vec3 g_LineEnds[6] = vec3[]
(
	vec3(-g_AxisLength, 0, 0),
	vec3(0, 0, 0),
	vec3(0, 0, 0),
	vec3(0, g_AxisLength, 0),
	vec3(0, 0, 0),
	vec3(0, 0, g_AxisLength)
);

void main() 
{
	VertexID = gl_VertexID;

	vec3 pos = g_LineEnds[gl_VertexID];
	gl_Position = g_ScreenOffset + g_WorldToViewMatrixNoTranslation * vec4(pos, 1);


	//gl_Position.w = 1;
}
