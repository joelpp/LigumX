#version 430 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

uniform mat4 vpMat;
uniform mat4 g_ModelToWorldMatrix;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_ProjectionMatrix;
uniform mat4 g_LightProjectionMatrix;

layout(binding = 3) uniform sampler2D g_HeightfieldTexture;


out gl_PerVertex {
    vec4 gl_Position;
};

out float v_Height;
out vec2 v_TexCoords;
out vec3 v_Normal;
out float v_maxHeight;

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

	vec4 worldPosition = g_ModelToWorldMatrix * vec4(tempPos, 1);

	// todo : find a better way to generate terrain normals...

	vec2 heightTexCoords = texCoord;
	heightTexCoords.x = 1.f - heightTexCoords.x;

	float heightMid = textureLod(g_HeightfieldTexture, heightTexCoords, 0.f).r;

	float resolution = 1.f / 64;
	vec3 normal = ComputeNormal(heightMid, heightTexCoords, resolution);

	worldPosition.z += heightMid;

	v_Height = heightMid;
	v_TexCoords = heightTexCoords;
	v_Normal = normal;

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * worldPosition;
	gl_Position.z += 0.1f;
}
