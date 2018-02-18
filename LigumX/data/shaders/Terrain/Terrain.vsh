#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

uniform mat4 vpMat;
uniform mat4 g_ModelToWorldMatrix;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_ProjectionMatrix;
uniform mat4 g_LightProjectionMatrix;

uniform sampler2D g_HeightfieldTexture;



float g_MaxHeight = 25.f; 



out gl_PerVertex {
    vec4 gl_Position;
};

out float v_Height;
out vec2 v_TexCoord;
out vec3 v_Normal;
out float v_maxHeight;

vec3 ComputeNormal(float heightMid, vec2 texCoord, float resolution)
{
	vec2 offsets = vec2(resolution);
	if (texCoord.x <= resolution + 0.001f)
	{
		offsets.x = 0;
	}
	offsets.y = 0;
	float heightLeft = textureLod(g_HeightfieldTexture, texCoord - offsets, 0.f).r;
	
	offsets = vec2(resolution);
	if (texCoord.x >= (1.f - resolution - 0.001f))
	{
		offsets.x = 0;
	}
	offsets.y = 0;
	float heightRight =		textureLod(g_HeightfieldTexture, texCoord + offsets * vec2(1, 0), 0.f).r;
	
	offsets = vec2(resolution);
	if (texCoord.y <= resolution + 0.001f)
	{
		offsets.y = 0;
	}
	offsets.x = 0;
	float heightBottom =	textureLod(g_HeightfieldTexture, texCoord - offsets * vec2(0, 1), 0.f).r;

	offsets = vec2(resolution);
	if (texCoord.y >= (1.f - resolution - 0.001f))
	{
		offsets.y = 0;
	}

	offsets.x = 0;
	float heightTop =		textureLod(g_HeightfieldTexture, texCoord + offsets * vec2(0, 1), 0.f).r;

	vec3 normal = vec3(0, 0, 0);

	resolution = 0.1f;
	float leftSlope		= -(heightMid - heightLeft) / sqrt(pow(resolution, 2) + pow(heightMid - heightLeft, 2)); 
	float rightSlope	= -(heightRight - heightMid) / sqrt(pow(resolution, 2) + pow(heightRight - heightMid, 2)); 
	normal.x += (leftSlope + rightSlope);

	float upSlope	= -(heightMid - heightBottom) / sqrt(pow(resolution, 2) + pow(heightMid - heightBottom, 2)); 
	float downSlope = -(heightTop - heightMid) / sqrt(pow(resolution, 2) + pow(heightTop - heightMid, 2)); 
	normal.y += (upSlope + downSlope);

	normal.x = clamp(normal.x, -1.f, 1.f);
	normal.y = clamp(normal.y, -1.f, 1.f);

	float hypotenuse = 0.5f;
	normal.z = max(sqrt(pow(hypotenuse, 2) - pow(normal.x, 2) - pow(normal.y, 2)), 1e-6);

	normal = normalize(normal);

	return normal;
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

	float resolution = 1.f / 512;


	// todo : find a better way to generate terrain normals...

	vec2 heightTexCoords = texCoord;
	heightTexCoords.y = 1.f - heightTexCoords.y;
	


	float heightMid = textureLod(g_HeightfieldTexture, heightTexCoords, 0.f).r;

	vec3 normal = ComputeNormal(heightMid, texCoord, resolution);

	heightMid*= g_MaxHeight;

	worldPosition.z += heightMid;

	v_Height = heightMid;
	v_TexCoord = heightTexCoords;
	v_maxHeight = g_MaxHeight;
	v_Normal = normal;

	gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * worldPosition;

}
