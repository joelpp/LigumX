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
uniform int g_DebugUVEnabled;
uniform int g_DebugLinearizeDepth;
#endif

#ifdef PROVIDER_PostEffects
uniform int g_GammaCorrectionEnabled;
uniform float g_GammaCorrectionExponent;
uniform bool g_ToneMappingEnabled;
#endif


#ifdef PROVIDER_ShadowMap
uniform sampler2D g_DepthMapTexture;
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
uniform vec2 g_MouseCoords;
#endif