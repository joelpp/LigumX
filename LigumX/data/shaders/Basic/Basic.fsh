#version 430 core

#define DEFERRED 0

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;

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

uniform vec4 g_DebugVec4;

//
//bool OutputDebugColors(inout PixelData pixelData, in vec3 screenCoords)
//{
//	if (g_DebugUVsEnabled > 0)
//	{
//		vec2 fractUV = fract(pixelData.m_UVs);
//		pixelData.m_FinalColor = vec4(fractUV.r, fractUV.g, 0.f, 1.f);
//		return true;
//	}
//
//	if (g_DebugDepthEnabled > 0)
//	{
//		float depth = 0;
//		if (g_DebugLinearizeDepth > 0)
//		{
//			depth = LinearizeDepth(screenCoords.z, g_CameraNearPlane, g_CameraFarPlane) / g_CameraFarPlane;
//		}
//		else
//		{
//			depth = gl_FragCoord.z;
//		}
//		FinalColor = vec4(depth, depth, depth, 1.f);
//		return true;
//	}
//
//	if (g_DebugNormalsEnabled > 0)
//	{
//		FinalColor = GetDebugNormalColor(pixelData.m_Normal);
//		return true;
//	}
//	return false;
//}



void main() 
{
	vec3 normal = normalize(vNormalWS);
	GBuffer_WriteNormal(normal);

	vec3 materialAlbedo = GetDiffuseColor(myTexCoord).rgb;
	GBuffer_WriteAlbedo(materialAlbedo);

	GBuffer_WriteRoughness(GetRoughness(myTexCoord));

	GBuffer_WriteWorldPosition(vWorldPosition.xyz);

	GBuffer_WriteMetallic(GetMetallic(myTexCoord));

}