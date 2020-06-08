#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

//in int gl_VertexID;
uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_View

// Include ProvidersMarker


out vec3 normal;
out vec3 direction;
out vec2 uv;
out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	direction = pos;
	uv = texCoord;
	normal = v_Normal;
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrixRotationOnly * vec4(pos, 1);;

}
