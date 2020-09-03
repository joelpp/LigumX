#version 430 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_ShadowMap
#define PROVIDER_View

// Include ProvidersMarker

out vec2 myTexCoord;
out vec3 vNormalWS;
out vec4 vWorldPosition;
out float height;
out vec4 FragPosCascade0;
out vec4 FragPosCascade1;
out vec4 FragPosCascade2;
out vec4 FragPosCascade3;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	vWorldPosition = g_ModelToWorldMatrix * vec4(pos, 1);
	//vWorldPosition = vec4(pos, 1);
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vWorldPosition;
    myTexCoord = texCoord;
	height = pos.z;
	vNormalWS = mat3(transpose(inverse(g_ModelToWorldMatrix))) * v_Normal;

	FragPosCascade0 = g_Cascade0ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade1 = g_Cascade1ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade2 = g_Cascade2ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade3 = g_Cascade3ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
}
