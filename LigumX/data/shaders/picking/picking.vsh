#version 420 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 vpMat;
uniform mat4 g_ModelToWorldMatrix;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_ProjectionMatrix;
uniform int g_UseHeightfield;

layout(binding = 3) uniform sampler2D g_HeightfieldTexture;


// Include ProvidersMarker

out vec3 vWorldPosition;

out gl_PerVertex {
    vec4 gl_Position;
};


void main() {
	vec4 worldPosition = g_ModelToWorldMatrix * vec4(pos, 1);

	if (g_UseHeightfield == 1)
	{
		vec2 heightTexCoords = texCoord;
		heightTexCoords.x = 1.f - heightTexCoords.x;

		float heightMid = textureLod(g_HeightfieldTexture, heightTexCoords, 0.f).r;

		worldPosition.z += heightMid;
	}

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * worldPosition;
	gl_Position.y = 990 - gl_Position.y;


	vWorldPosition = worldPosition.xyz;
}
