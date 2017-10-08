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

layout (location = 0) out vec4 FinalColor;
layout (location = 1) out vec4 BrightColor;


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
	if (!g_UseShadows)
	{
		return 0.f;
	}
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

vec4 SampleTexture(sampler2D theTexture, vec2 uv)
{
	vec4 samplingResult = texture(theTexture, uv);

	return samplingResult;
}

vec4 GetDiffuseColor(vec2 uv)
{
	vec4 diffuseColor;
	if (g_Material.m_DiffuseTextureEnabled)
	{
		diffuseColor = SampleTexture(g_Material.m_DiffuseTexture, uv);

		if (g_GammaCorrectionEnabled > 0)
		{
			diffuseColor.rgb = pow(diffuseColor.rgb, vec3(g_GammaCorrectionExponent));
		}
	} 
	else
	{
		diffuseColor = vec4(g_Material.m_DiffuseColor, 1.0f);
	}

	return diffuseColor;
}

vec3 GetLightColor()
{
	vec3 lightColor;
	if (g_UseSkyLighting)
	{
		lightColor = g_DirectionalLight.m_DiffuseColor;
	}
	else
	{
		lightColor = g_PointLight.m_DiffuseColor;
	}

	return lightColor;
}

vec3 GetLightDirection()
{
	vec3 lightDirection;
	if (g_UseSkyLighting)
	{
		lightDirection = g_DirectionalLight.m_Direction;
	}
	else
	{
		lightDirection = g_PointLight.m_Position - vWorldPosition.xyz;
	}

	return lightDirection;
}

vec3 GetDiffuseLighting(vec3 fragmentToLight, vec3 fNormalWS)
{
 	// Diffuse
	vec4 diffuseColor = GetDiffuseColor(myTexCoord);

	if (g_Material.m_Unlit)
	{
		return diffuseColor.rgb;
	}

	float diffuseFactor = max(0.f, dot(fragmentToLight, fNormalWS));

	return diffuseFactor * diffuseColor.rgb * GetLightColor();
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
		vec3 fragmentToLight = normalize( GetLightDirection() );
		vec3 reflectionDir = reflect(-fragmentToLight, fNormalWS);

		// Ambient
		vec3 ambientContribution = g_PointLight.m_AmbientColor * g_Material.m_AmbientColor; 
		ambientContribution *= g_DebugAmbientEnabled;

		vec3 diffuseContribution = GetDiffuseLighting(fragmentToLight, fNormalWS);

		// Specular
		vec4 specularColor = vec4(0.5,0.5,0.5,1);//texture(g_Material.m_SpecularTexture, myTexCoord);
		float spec = 0;
		if (g_BlinnPhongShading > 0)
		{
			vec3 halfwayVector = normalize(fragmentToLight + fragmentToCamera);
			spec = pow(max(dot(fNormalWS, halfwayVector), 0.0), g_Material.m_Shininess);
		}
		else
		{
			spec = pow(max(dot(fragmentToCamera, reflectionDir), 0.0), g_Material.m_Shininess);
		}
		vec3 specularContribution = spec * specularColor.rgb;  
		specularContribution *= g_DebugSpecularEnabled;

		float shadow = ShadowCalculation(FragPosLightSpace, fNormalWS);
		// final 
		FinalColor.rgb = /*ambientContribution +*/ (1.0 - shadow) * diffuseContribution /*+ specularContribution*/;
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
	
	BrightColor = FinalColor * g_Material.m_EmissiveFactor;
}