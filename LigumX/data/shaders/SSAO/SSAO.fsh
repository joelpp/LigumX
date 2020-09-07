#version 440 core

#define DEFERRED_RESOLVE

in vec4 gl_FragCoord;
in vec2 uv;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D g_GBuffer0;
layout(binding = 1) uniform sampler2D g_GBuffer1;
layout(binding = 2) uniform sampler2D g_GBuffer2;
layout(binding = 3) uniform sampler2D g_GBuffer3;
layout(binding = 4) uniform sampler2D g_GBufferDepth;
layout(binding = 6) uniform sampler2D g_NoiseTexture;
//in vec4 FragPosCascade0;
//in vec4 FragPosCascade1;
//in vec4 FragPosCascade2;
//in vec4 FragPosCascade3;

uniform vec3 g_SSAOKernelSamples[64];

#define PROVIDER_View
#define PROVIDER_ShadowMap
#define PROVIDER_Sky
#define PROVIDER_DisplayOptions
#define PROVIDER_Debug
#define PROVIDER_LightingOptions
#define PROVIDER_Light
#define PROVIDER_Material
#define PROVIDER_PostEffects

// Include ProvidersMarker


// Include BasicUtils.glsl
// Include ShadowUtils.glsl
// Include LightUtils.glsl
// Include MaterialUtils.glsl
// Include LightingUtils.glsl
// Include SkyUtils.glsl




void main()
{
	vec4 gBuffer0 = SampleTexture(g_GBuffer0, uv);
	vec4 gBuffer1 = SampleTexture(g_GBuffer1, uv);
	vec4 gBuffer2 = SampleTexture(g_GBuffer2, uv);
	vec4 gBuffer3 = SampleTexture(g_GBuffer3, uv);
	float gBufferDepth = SampleTexture(g_GBufferDepth, uv).r;

	// todo jpp : should be done with stencil maybe?
	if (length(gBuffer0) == 0)
	{
		discard;
	}

	vec3 wsNormal = (gBuffer0.rgb * 2.f) - vec3(1.f);

	float metallic = gBuffer0.a;
	float roughness = gBuffer1.a;
	vec3 worldPosition = gBuffer1.rgb;
	vec3 materialAlbedo = gBuffer2.rgb;

	vec3 fragmentToCamera = normalize(g_CameraPosition - worldPosition.xyz);

	// todo jpp better noise tiling
	vec4 noise = SampleTexture(g_NoiseTexture, uv * 100.f);
	vec3 randomVec = noise.xyz;
	vec3 normal = wsNormal;
	vec3 fragPos = (g_WorldToViewMatrix2 * vec4(worldPosition, 1.f)).xyz;
	//vec3 fragPos = worldPosition.xyz;
	float bias = 0.025f;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.f;
	int kernelSize = 64;
	float radius = 0.2f;

	for (int i = 0; i < kernelSize; ++i)
	{
		//get sample position
		vec3 samplePos = TBN* g_SSAOKernelSamples[i]; // from tangent to view-space
		vec3 worldSample = worldPosition + samplePos * radius;

		vec4 offset = g_WorldToViewMatrix2 * vec4(worldSample, 1.0);
		vec4 ov = offset;
		offset = g_ProjectionMatrix * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  

		vec4 offsetWorldPos = texture(g_GBuffer1, offset.xy);
		vec3 offsetCamPos = (g_WorldToViewMatrix2 * vec4(offsetWorldPos.xyz, 1.f)).xyz;;
		float sampleDepth = offsetCamPos.z;

		//occlusion += (sampleDepth >= fragPos.z + bias ? 1.0 : 0.0);
		//occlusion += sampleDepth;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= ov.z + bias ? 1.0 : 0.0) * rangeCheck;
		//FragColor = vec4(worldSample, 1.f);

	}

	occlusion = 1.0 - (occlusion / float(kernelSize));
	//occlusion /= 5.f;
	FragColor = vec4(occlusion, occlusion, occlusion, 1.f);

}

