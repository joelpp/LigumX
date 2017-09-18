#version 410 core
#extension GL_ARB_shading_language_include : require

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

#define PROVIDER_View
#define PROVIDER_Light
#define PROVIDER_Debug
#define PROVIDER_PostEffects
#define PROVIDER_Material


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


out vec4 FinalColor;

vec4 GetDebugNormalColor(vec3 normalWS)
{
	return vec4(0.5f * (normalWS + vec3(1,1,1)), 1.f);
}

float LinearizeDepth(float nonLinearDepth)
{
	float near = g_CameraNearPlane;
	float far = g_CameraFarPlane;

	float z = nonLinearDepth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() 
{
			float depth = gl_FragCoord.z;
		FinalColor = vec4(depth, depth, 0.8, 1.f);
		return ;

	vec3 fNormalWS = normalize(vNormalWS);
	

	if (g_DebugNormalsEnabled > 0)
	{
		FinalColor = GetDebugNormalColor(fNormalWS);
		return ;
	}

	//if (g_DebugDepthEnabled > 0)
	{
		//float depth = 0;
		//if (g_DebugLinearizeDepth > 0)
		//{
		//	depth = LinearizeDepth(gl_FragCoord.z) / g_CameraFarPlane;
		//}
		//else
		//{
			depth = gl_FragCoord.z;
		//}
		FinalColor = vec4(depth, depth, 0.8, 1.f);
		return ;
	}

	// todo : when i do proper attenuation i need to re-visit gamma

	if (g_UseLighting > 0)
	{
		// Directions
		vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
		//vec3 fragmentToLight = normalize( g_PointLight.m_Position - vWorldPosition.xyz);
		vec3 fragmentToLight = normalize( -g_DirectionalLight.m_Direction );
		vec3 reflectionDir = reflect(-fragmentToLight, fNormalWS);

		// Ambient
		vec3 ambientContribution = g_PointLight.m_AmbientColor * g_Material.ambient; 
		ambientContribution *= g_DebugAmbientEnabled;

		// Diffuse
		vec4 diffuseColor = texture(g_Material.m_DiffuseTexture, myTexCoord);
		//if (g_GammaCorrectionEnabled > 0)
		//{
		//	diffuseColor.rgb = pow(diffuseColor.rgb, vec3(g_GammaCorrectionExponent));
		//}

		float diffuseFactor = max(0.f, dot(fragmentToLight, fNormalWS));
		vec3 diffuseContribution = /*diffuseFactor * */diffuseColor.rgb;
		//diffuseContribution = normalize( /*g_PointLight.m_Position - */vWorldPosition.xyz);;
		//diffuseContribution *= g_DebugDiffuseEnabled;

		// Specular
		vec4 specularColor = texture(g_Material.m_SpecularTexture, myTexCoord);
		float spec = 0;
		if (g_BlinnPhongShading > 0)
		{
			vec3 halfwayVector = normalize(fragmentToLight + fragmentToCamera);
			spec = pow(max(dot(fNormalWS, halfwayVector), 0.0), g_Material.shininess);
		}
		else
		{
			spec = pow(max(dot(fragmentToCamera, reflectionDir), 0.0), g_Material.shininess);
		}
		vec3 specularContribution = spec * specularColor.rgb;  
		specularContribution *= g_DebugSpecularEnabled;


		// final 
		FinalColor.rgb = ambientContribution + diffuseContribution /*+ specularContribution*/;
	}
	else
	{
		vec4 diffuseColor = texture(g_Material.m_DiffuseTexture, myTexCoord);
		FinalColor = diffuseColor;
	}

	if (g_GammaCorrectionEnabled > 0)
	{
		FinalColor.rgb = pow(FinalColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
	}

}