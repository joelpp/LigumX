#version 410 core

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosLightSpace;


#define PROVIDER_View
#define PROVIDER_Light
#define PROVIDER_Debug
#define PROVIDER_PostEffects
#define PROVIDER_Material
#define PROVIDER_ShadowMap
// Include Providers Marker


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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normalWS)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 


	float closestDepth = texture(g_DepthMapTexture, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.005);  
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
		if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main() 
{
	vec3 fNormalWS = normalize(vNormalWS);
	
	if (g_DebugUVEnabled > 0)
	{
		FinalColor = vec4(myTexCoord.x, myTexCoord.y, 0, 1.f);
		return ;
	}


	if (g_DebugNormalsEnabled > 0)
	{
		FinalColor = GetDebugNormalColor(fNormalWS);
		return ;
	}

	if (g_DebugDepthEnabled > 0)
	{
		float depth = 0;
		if (g_DebugLinearizeDepth > 0)
		{
			depth = LinearizeDepth(gl_FragCoord.z) / g_CameraFarPlane;
		}
		else
		{
			depth = gl_FragCoord.z;
		}
		FinalColor = vec4(depth, depth, depth, 1.f);
		return ;
	}

	// todo : when i do proper attenuation i need to re-visit gamma

	if (g_UseLighting > 0)
	{
		// Directions
		vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);
		//vec3 fragmentToLight = normalize( g_PointLight.m_Position - vWorldPosition.xyz);
		vec3 fragmentToLight = normalize( g_DirectionalLight.m_Direction );
		vec3 reflectionDir = reflect(-fragmentToLight, fNormalWS);

		// Ambient
		vec3 ambientContribution = g_PointLight.m_AmbientColor * g_Material.ambient; 
		ambientContribution *= g_DebugAmbientEnabled;

		// Diffuse
		vec4 diffuseColor = texture(g_Material.m_DiffuseTexture, myTexCoord) ;
		if (g_GammaCorrectionEnabled > 0)
		{
			diffuseColor.rgb = pow(diffuseColor.rgb, vec3(g_GammaCorrectionExponent));
		}

		float diffuseFactor = max(0.f, dot(fragmentToLight, fNormalWS));
		vec3 diffuseContribution = diffuseFactor * diffuseColor.rgb;
		diffuseContribution *= g_DebugDiffuseEnabled;

		// Specular
		vec4 specularColor = vec4(0.5,0.5,0.5,1);//texture(g_Material.m_SpecularTexture, myTexCoord);
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

		float shadow = ShadowCalculation(FragPosLightSpace, fNormalWS);
		// final 
		FinalColor.rgb = ambientContribution + (1.0 - shadow) * diffuseContribution + specularContribution;
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