#version 430 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_ShadowMap
#define PROVIDER_View
#define PROVIDER_Debug

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

	float amplitude = 0.05f;

	float s = amplitude * 0.5f * (1.f + sin(1.f * vWorldPosition.x + g_CurrentFrame / 8.f));
	float c = 8.f * amplitude * 0.5f * (1.f + cos(100.f * vWorldPosition.y + g_CurrentFrame / 50.6f));
	float waterHeight = s * c;
	vWorldPosition.z += waterHeight;
	//vWorldPosition = vec4(pos, 1);
	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vWorldPosition;
    myTexCoord = texCoord;
	height = pos.z;

	float resolution = 1.f;
	float dx0 = amplitude * 0.5f * (1.f + sin(1.f * (vWorldPosition.x - resolution) + g_CurrentFrame / 8.f));
	float dx1 = amplitude * 0.5f * (1.f + sin(1.f * (vWorldPosition.x + resolution) + g_CurrentFrame / 8.f));

	float dy0 = 8.f * amplitude * 0.5f * (1.f + cos(100.f * (vWorldPosition.y - resolution) + g_CurrentFrame / 50.6f));
	float dy1 = 8.f * amplitude * 0.5f * (1.f + cos(100.f * (vWorldPosition.y + resolution) + g_CurrentFrame / 50.6f));

	vec3 dnx = vec3(2.f * resolution, 0.f, (dx1 - dx0));
	vec3 dny = vec3(0.f, 2.f * resolution, (dy1 - dy0));
	vNormalWS = normalize(cross(dnx, dny));

	//float dhdv = (2 * resolution) * (heightTop - heightBottom);

	//vNormalWS = mat3(transpose(inverse(g_ModelToWorldMatrix))) * v_Normal;

	FragPosCascade0 = g_Cascade0ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade1 = g_Cascade1ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade2 = g_Cascade2ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
	FragPosCascade3 = g_Cascade3ProjectionMatrix * vec4(vWorldPosition.xyz, 1.f);
}
