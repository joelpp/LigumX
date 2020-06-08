#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_View

// Include ProvidersMarker

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * g_ModelToWorldMatrix * vec4(pos, 1);
}
