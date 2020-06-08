#ifndef PROVIDER_DEFINITIONS_H
#define PROVIDER_DEFINITIONS_H

#define PI 3.141592654f

#ifdef PROVIDER_Material
struct Material
{
	vec3		m_AmbientColor;
	vec3		m_DiffuseColor;
	vec3		m_SpecularColor;
	float		m_Shininess;
	sampler2D	m_DiffuseTexture;
	sampler2D	m_SpecularTexture;
	bool		m_DiffuseTextureEnabled;
	bool		m_SpecularTextureEnabled;
	bool		m_Unlit;
	float		m_EmissiveFactor;
	bool		m_IsGlass;
	float		m_RefractionIndex;
	bool		m_ReflectEnvironment;
	float		m_Metallic;
	float		m_Roughness;
	float		m_AO;
	bool		m_IsPBR;
	vec2   m_UVScale;
};

uniform Material g_Material;
uniform int g_BlinnPhongShading;

#endif

#ifdef PROVIDER_Light
struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_DiffuseColor;
	vec3 m_AmbientColor;
	vec3 m_SpecularColor;
};

uniform DirectionalLight g_DirectionalLight;

struct PointLight
{
	vec3 m_Position;
	vec3 m_DiffuseColor;
	vec3 m_AmbientColor;
	vec3 m_SpecularColor;
};

uniform PointLight g_PointLight[8];

uniform int g_NumLights;

uniform int g_UseLighting;
uniform bool g_UseSkyLighting;
uniform bool g_UseShadows;
#endif

#ifdef PROVIDER_View
uniform vec3 g_CameraPosition;
uniform vec3 g_CameraLookAt;
uniform float g_CameraNearPlane;
uniform float g_CameraFarPlane;
uniform mat4 g_CameraInverse;
uniform mat4 g_WorldToViewMatrix;
uniform mat4 g_WorldToViewMatrixNoTranslation;
uniform mat4 g_ProjectionMatrix;
uniform mat4 g_ViewProjectionMatrixInverse;
uniform mat4 g_ViewMatrixInverse;
uniform mat4 g_ProjectionMatrixInverse;
#endif

#ifdef PROVIDER_Debug
uniform int g_DebugDiffuseEnabled;
uniform int g_DebugSpecularEnabled;
uniform int g_DebugAmbientEnabled;
uniform int g_DebugNormalsEnabled;
uniform int g_DebugDepthEnabled;
uniform int g_DebugUVsEnabled;
uniform int g_DebugLinearizeDepth;
#endif

#ifdef PROVIDER_LightingOptions
uniform bool g_EnableDynamicLights;
uniform bool g_EnableSunlight;
uniform bool g_EnableAmbientLighting;
uniform bool g_EnableDiffuseComponent;
uniform bool g_EnableSpecularComponent;
uniform bool g_EnableReflection;
uniform bool g_EnableSunShadow;
uniform bool g_EnableDynamicShadows;
uniform bool g_EnableReplacementAlbedo;
uniform vec3 g_ReplacementAlbedo;
uniform bool g_EnableReplacementMetallic;
uniform float g_ReplacementMetallic;
uniform bool g_EnableReplacementRoughness;
uniform float g_ReplacementRoughness;
#endif // PROVIDER_LightingOptions

#ifdef PROVIDER_PostEffects
uniform int g_GammaCorrectionEnabled;
uniform float g_GammaCorrectionExponent;
uniform bool g_ToneMappingEnabled;
#endif


#ifdef PROVIDER_ShadowMap
uniform sampler2D g_DepthMapTexture;
uniform mat4 g_LightProjectionMatrix;
#endif

#ifdef PROVIDER_Depth
layout(binding=0) uniform sampler2D g_DepthTexture;
#endif

#ifdef PROVIDER_Picking
uniform float g_PickingID;
#endif

#ifdef PROVIDER_Sky
uniform bool g_UseSkybox;
uniform samplerCube g_Skybox;
uniform float sunOrientation;
uniform float sunTime;
#endif

#ifdef PROVIDER_Window
uniform vec2 g_WindowSize;
#endif

#ifdef PROVIDER_WorldGrid
uniform float g_WorldScale;
uniform vec3 g_SectorGridColor;
#endif

#ifdef PROVIDER_DisplayOptions

// todo autogen from cpp enum...
#define DisplayMode_Final 0
#define DisplayMode_Depth 1
#define DisplayMode_UV 2
#define DisplayMode_Diffuse 3
#define DisplayMode_Normals 4

uniform int g_DisplayMode;
#endif

#ifdef PROVIDER_View
float LinearizeDepth(float nonLinearDepth)
{
	float near = g_CameraNearPlane;
	float far = g_CameraFarPlane;

	float z = nonLinearDepth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}
#endif


vec4 GetDebugNormalColor(vec3 normalWS)
{
	return vec4(0.5f * (normalWS + vec3(1, 1, 1)), 1.f);
}

struct PixelData
{
	vec4 m_FinalColor;
	vec4 m_DiffuseColor;
	vec3 m_Normal;
	float m_Depth;
	vec2 m_UVs;
};

vec3 BuildShaderOutput(PixelData pixelData)
{

#ifdef PROVIDER_DisplayOptions
	if (g_DisplayMode == DisplayMode_Final)
	{
		return pixelData.m_FinalColor.rgb;
	}
	else if (g_DisplayMode == DisplayMode_Depth)
	{
		return vec3(pixelData.m_Depth, pixelData.m_Depth, pixelData.m_Depth);
	}
	else if (g_DisplayMode == DisplayMode_UV)
	{
		return vec3(pixelData.m_UVs, 0);
	}
	else if (g_DisplayMode == DisplayMode_Diffuse)
	{
		return pixelData.m_DiffuseColor.rgb;
	}
	else if (g_DisplayMode == DisplayMode_Normals)
	{
		return GetDebugNormalColor(pixelData.m_Normal.rgb).rgb;
	}
#endif

	return pixelData.m_FinalColor.rgb;
}

#ifdef PROVIDER_DataInspector
//#define DATAINSPECTOR_BINDPOS 7 // keep in sync with cpp
layout(std430, binding = 7) restrict buffer g_DataInspectorLayout
{
	float g_InspectorData[];
};
uniform int g_MouseX;
uniform int g_MouseY;

void DebugWatch(ivec2 pos, uint index, float value)
{
	if ((g_MouseX == pos.x) && (g_MouseY == pos.y))
	{
		g_InspectorData[index] = value;
	}
}

void DebugWatch(vec2 fragCoords, uint index, float value)
{
	DebugWatch(ivec2(fragCoords.xy), index, value);
}

void DebugWatch(vec4 fragCoords, uint index, float value)
{
	DebugWatch(ivec2(fragCoords.xy), index, value);
}

void DebugWatch(uint index, float value)
{
	DebugWatch(gl_FragCoord, index, value);

}
#endif

#endif // PROVIDER_DEFINITIONS_H