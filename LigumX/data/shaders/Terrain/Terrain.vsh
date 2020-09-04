#version 430 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

//uniform int g_TerrainPatchIndex;

layout(std430, binding = 2) buffer g_TerrainPatchMatrices
{
	mat4 m_Data[];
};

layout(binding = 3) uniform sampler2D g_HeightfieldTexture;

#define PROVIDER_View
#define PROVIDER_ShadowMap

// Include ProvidersMarker

out gl_PerVertex {
    vec4 gl_Position;
};

out float v_Height;
out vec2 v_TexCoords;
out vec3 v_Normal;
out float v_maxHeight;
out vec4 FragPosCascade0;
out vec4 FragPosCascade1;
out vec4 FragPosCascade2;
out vec4 FragPosCascade3;
out vec4 vWorldPosition;

vec3 ComputeNormal(float heightMid, vec2 texCoord, float resolution)
{
	vec2 du = vec2(resolution, 0);
	vec2 dv = vec2(0, resolution);

	float heightLeft = textureLod(g_HeightfieldTexture, texCoord - du, 0.f).r;

	float heightRight =	textureLod(g_HeightfieldTexture, texCoord + du, 0.f).r;

	float heightBottom = textureLod(g_HeightfieldTexture, texCoord - dv , 0.f).r;
	float heightTop = textureLod(g_HeightfieldTexture, texCoord + dv , 0.f).r;

	float dhdu = (2 * resolution) * (heightRight - heightLeft); 
	float dhdv = (2 * resolution) * (heightTop - heightBottom); 

	//vec3 normal = normalize(vec3(0,0,1) + vec3(1,0,0) * dhdu + vec3(0,1,0) * dhdv);

	vec3 va = normalize(vec3(2, 0, heightRight - heightLeft));
    vec3 vb = normalize(vec3(0, 2, heightTop - heightBottom));
    vec3 normal = cross(va,vb);

	return normalize(normal);
}

float srgbToLinear(float S)
{
return S;
	return pow( (S + 0.055f) / 1.055f , 2.4f);
}
void main()
{ 
	vec3 tempPos = pos;
	uint instanceID = gl_InstanceID;

	mat4 modelToWorld = m_Data[instanceID];
	vec4 worldPosition = modelToWorld * vec4(tempPos, 1);
	// todo : find a better way to generate terrain normals...

	vec2 heightTexCoords = texCoord;

	float heightMid = textureLod(g_HeightfieldTexture, heightTexCoords, 0.f).r;

	float resolution = 1.f / 64;
	vec3 normal = ComputeNormal(heightMid, heightTexCoords, resolution);

	//worldPosition.z += heightMid;

	v_Height = heightMid;
	v_TexCoords = heightTexCoords / 0.01135;
	v_Normal = normal;

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * worldPosition;
	//gl_Position.z += 0.1f;

	FragPosCascade0 = g_Cascade0ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
	FragPosCascade1 = g_Cascade1ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
	FragPosCascade2 = g_Cascade2ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
	FragPosCascade3 = g_Cascade3ProjectionMatrix * vec4(worldPosition.xyz, 1.f);

	vWorldPosition = worldPosition;

}
