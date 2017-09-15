#version 410 core
in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D m_DiffuseTexture;
    sampler2D m_SpecularTexture;
}; 
  
uniform Material g_Material;

struct Light
{
	vec3 m_Position;
	vec3 m_DiffuseColor;
	vec3 m_AmbientColor;
	vec3 m_SpecularColor;
};

uniform Light g_Light;


uniform int g_UseLighting;
uniform sampler2D g_Diffuse;

uniform vec3 g_CameraPosition;

uniform int g_DebugDiffuseEnabled;
uniform int g_DebugSpecularEnabled;
uniform int g_DebugAmbientEnabled;
uniform int g_DebugNormalsEnabled;

out vec4 FinalColor;
void main() 
{
	
	vec3 fNormalWS = normalize(vNormalWS);


	if (g_UseLighting > 0)
	{

		// Directions
		vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
		vec3 fragmentToLight = normalize( g_Light.m_Position - vWorldPosition.xyz);
		vec3 reflectionDir = reflect(-fragmentToLight, fNormalWS);

		// Ambient
		vec3 ambientContribution = g_Light.m_AmbientColor * g_Material.ambient; 
		ambientContribution *= g_DebugAmbientEnabled;

		// Diffuse
		vec4 diffuseColor = texture(g_Material.m_DiffuseTexture, myTexCoord);
		float diffuseFactor = max(0.f, dot(fragmentToLight, fNormalWS));
		vec3 diffuseContribution = diffuseFactor * diffuseColor.rgb;
		//diffuseContribution = normalize( /*g_Light.m_Position - */vWorldPosition.xyz);;
		diffuseContribution *= g_DebugDiffuseEnabled;

		// Specular
		vec4 specularColor = texture(g_Material.m_SpecularTexture, myTexCoord);
		float spec = pow(max(dot(fragmentToCamera, reflectionDir), 0.0), g_Material.shininess);
		vec3 specularContribution = spec * specularColor.rgb;  
		specularContribution *= g_DebugSpecularEnabled;


		// final 
		FinalColor.rgb = ambientContribution + diffuseContribution + specularContribution;
	}
	else
	{
		vec4 diffuseColor = texture(g_Material.m_DiffuseTexture, myTexCoord);
		FinalColor = diffuseColor;
	}

	if (g_DebugNormalsEnabled > 0)
	{
		FinalColor.rgb = 0.5f * (fNormalWS + vec3(1,1,1));
	}
		FinalColor.a = 1.0f;
}