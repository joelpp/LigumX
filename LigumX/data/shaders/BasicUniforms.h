
#ifdef PROVIDER_Material
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	sampler2D m_DiffuseTexture;
	sampler2D m_SpecularTexture;
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

uniform PointLight g_PointLight;

uniform int g_UseLighting;
#endif

#ifdef PROVIDER_View
uniform vec3 g_CameraPosition;
uniform float g_CameraNearPlane;
uniform float g_CameraFarPlane;
#endif

#ifdef PROVIDER_Debug
uniform int g_DebugDiffuseEnabled;
uniform int g_DebugSpecularEnabled;
uniform int g_DebugAmbientEnabled;
uniform int g_DebugNormalsEnabled;
uniform int g_DebugDepthEnabled;
uniform int g_DebugLinearizeDepth;
#endif

#ifdef PROVIDER_PostEffects
uniform int g_GammaCorrectionEnabled;
uniform float g_GammaCorrectionExponent;
#endif
