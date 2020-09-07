#version 430 core
in float v_Height;
in vec2 v_TexCoords;
in vec3 v_Normal;
in float v_maxHeight;
in vec4 FragPosCascade0;
in vec4 FragPosCascade1;
in vec4 FragPosCascade2;
in vec4 FragPosCascade3;
in vec3 vWorldPosition;

layout(location = 0) out vec4 outGBuffer0;
layout(location = 1) out vec4 outGBuffer1;
layout(location = 2) out vec4 outGBuffer2;
layout(location = 3) out vec4 outGBuffer3;

#define PROVIDER_PostEffects
#define PROVIDER_Material
#define PROVIDER_LightingOptions

// Include ProvidersMarker

// Include BasicUtils.glsl
// Include MaterialUtils.glsl
// Include GBufferUtils.glsl


layout(binding = 3) uniform sampler2D g_HeightfieldTexture;
layout(binding = 1) uniform sampler2D g_SplatMapTexture;


layout(binding = 4) uniform sampler2D g_DirtTexture;
layout(binding = 5) uniform sampler2D g_SandTexture;
layout(binding = 6) uniform sampler2D g_WoodTexture;
layout(binding = 7) uniform sampler2D g_AlbedoTexture;



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

	vec3 va = normalize(vec3(2, 0, 1.f / 25 * (heightRight - heightLeft)));
    vec3 vb = normalize(vec3(0, 2, 1.f / 25 * (heightTop - heightBottom)));
    vec3 normal = cross(va,vb);

	return normalize(normal);
}
vec3 blend(vec3 texture1, float a1, vec3 texture2, float a2)
{
    float depth = 0.2;
    float ma = max(a1, a2) - depth;

    float b1 = max(a1 - ma, 0);
    float b2 = max(a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}


//

void main() 
{
	vec3 normal = normalize(v_Normal);
	GBuffer_WriteNormal(normal);

	vec3 materialAlbedo = GetDiffuseColor(v_TexCoords).rgb;
	GBuffer_WriteAlbedo(materialAlbedo);

	GBuffer_WriteRoughness(GetRoughness(v_TexCoords));

	GBuffer_WriteWorldPosition(vWorldPosition.xyz);

	GBuffer_WriteMetallic(GetMetallic(v_TexCoords));
   
}