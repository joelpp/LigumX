#version 430 core

#define DEFERRED 0

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosLightSpace;


#define PROVIDER_View
#define PROVIDER_Light
#define PROVIDER_PostEffects
#define PROVIDER_Material
#define PROVIDER_ShadowMap
#define PROVIDER_Sky
#define PROVIDER_DisplayOptions
#define PROVIDER_Debug
#define PROVIDER_DataInspector

// Include ProvidersMarker
uniform vec4 g_DebugVec4;

#if DEFERRED
layout(location = 0) out vec4 Normal;
layout(location = 1) out vec4 WorldPosition;
#else
layout(location = 0) out vec4 FinalColor;
layout(location = 1) out vec4 BrightColor;
#endif


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normalWS, vec2 fragCoord)
{
	if (!g_UseShadows)
	{
		return 0.f;
	}
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//projCoords.xy *= g_DebugVec4.xy;
    projCoords = projCoords * 0.5 + 0.5; 


	float closestDepth = textureLod(g_DepthMapTexture, projCoords.xy, 0.f).r;

	float currentDepth = projCoords.z;

	float bias = max(0.01 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.005);
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

vec4 GetSpecularColor(vec2 uv)
{
	vec4 specularColor;
	if (g_Material.m_SpecularTextureEnabled)
	{
		specularColor = SampleTexture(g_Material.m_SpecularTexture, uv * g_Material.m_UVScale);
	} 
	else
	{
		specularColor = vec4(g_Material.m_SpecularColor, 1.0f);
	}

	return specularColor;
}


vec4 GetDiffuseColor(vec2 uv)
{
	vec4 diffuseColor;
	if (g_Material.m_DiffuseTextureEnabled)
	{
		diffuseColor = SampleTexture(g_Material.m_DiffuseTexture, uv * g_Material.m_UVScale);

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

vec3 GetLightColor(int lightIndex)
{
	vec3 lightColor;
	if (g_UseSkyLighting)
	{
		//lightColor = g_DirectionalLight.m_DiffuseColor;
		lightColor = vec3(1, 0, 0);
	}
	else
	{
		lightColor = g_PointLight[lightIndex].m_DiffuseColor;
	}

	return lightColor;
}

vec3 GetLightDirection(int lightIndex)
{
	vec3 lightDirection;
	if (g_UseSkyLighting)
	{
		//lightDirection = g_DirectionalLight.m_Direction;
		lightDirection = vec3(0, 0, 1);
	}
	else
	{
		lightDirection = g_PointLight[lightIndex].m_Position - vWorldPosition.xyz;
	}

	return lightDirection;
}

vec3 GetDiffuseLighting(int lightIndex, vec3 fragmentToLight, vec3 fNormalWS)
{
 	// Diffuse
	vec4 diffuseColor = GetDiffuseColor(myTexCoord);

	if (g_Material.m_Unlit)
	{
		return diffuseColor.rgb;
	}

	float diffuseFactor = max(0.f, dot(fragmentToLight, fNormalWS));

	return diffuseFactor * diffuseColor.rgb * GetLightColor(lightIndex);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 GetSpecularLighting(int lightIndex, vec3 fragmentToLight, vec3 fragmentToCamera, vec3 fNormalWS)
{
	// Specular
	vec4 specularColor = GetSpecularColor(myTexCoord);

	vec3 reflectionDir = reflect(-fragmentToLight, fNormalWS);
	vec3 halfwayVector = normalize(fragmentToLight + fragmentToCamera);
	float spec = 0;
	if (g_BlinnPhongShading > 0)
	{
		spec = pow(max(dot(fNormalWS, halfwayVector), 0.0), g_Material.m_Shininess);
	}
	else
	{
		spec = pow(max(dot(fragmentToCamera, reflectionDir), 0.0), g_Material.m_Shininess);
	}

	spec *= g_DebugSpecularEnabled;



	return spec * specularColor.rgb * GetLightColor(lightIndex);  
}

void main() 
{
	PixelData pixelData;

	pixelData.m_UVs = myTexCoord;
	pixelData.m_FinalColor.rgb = vec3(0,0,0);
	pixelData.m_Normal = normalize(vNormalWS);
	pixelData.m_Depth = LinearizeDepth(gl_FragCoord.z) / g_CameraFarPlane;

	vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);

	if (g_DebugUVsEnabled > 0)
	{
		vec2 fractUV = fract(pixelData.m_UVs);
		FinalColor = vec4(fractUV.r, fractUV.g, 0.f, 1.f);
		return;
	}

	
	//if (g_DebugDepthEnabled > 0)
	//{
	//	float depth = 0;
	//	if (g_DebugLinearizeDepth > 0)
	//	{
	//		depth = LinearizeDepth(gl_FragCoord.z) / g_CameraFarPlane;
	//	}
	//	else
	//	{
	//		depth = gl_FragCoord.z;
	//	}
	//	FinalColor = vec4(depth, depth, depth, 1.f);
	//	return ;
	//}

	// todo : when i do proper attenuation i need to re-visit gamma

	if (g_UseLighting > 0)
	{
#if 0
		for (int lightIndex = 0; lightIndex < g_NumLights; ++lightIndex)
		{
			if (!g_Material.m_IsPBR)
			{
				// Directions
				//vec3 fragmentToCamera = g_CameraPosition - vWorldPosition.xyz;
				//vec3 fragmentToLight = normalize( g_PointLight.m_Position - vWorldPosition.xyz);
				vec3 fragmentToLight = GetLightDirection(lightIndex);
				vec3 fragmentToLightDir = normalize(fragmentToLight);

				float lightDistance = length(fragmentToLight);

				float constant = 1.0f;
				float llinear = 0.09f;
				float quadratic = 0.032f;
				//float attenuation = 1.0 / (constant + llinear * lightDistance + quadratic * (lightDistance * lightDistance));    
				float attenuation = 1.0 / (lightDistance * lightDistance);
			
				// Ambient
				vec3 ambientContribution = g_PointLight[0].m_AmbientColor * g_Material.m_AmbientColor; 
				ambientContribution *= g_DebugAmbientEnabled;


				vec3 diffuseContribution = GetDiffuseLighting(lightIndex, fragmentToLightDir, pixelData.m_Normal);

			
				vec3 specularContribution = GetSpecularLighting(lightIndex, fragmentToLightDir, fragmentToCamera, pixelData.m_Normal);

				float shadow = ShadowCalculation(FragPosLightSpace, pixelData.m_Normal);
				// final 
				//FinalColor.rgb += /*ambientContribution +*/ (1.0 - shadow) * diffuseContribution + specularContribution;
				FinalColor.rgb += attenuation * (diffuseContribution + specularContribution);
			}
			else
			{
			   // calculate per-light radiance

					vec3 fragmentToLight = GetLightDirection(lightIndex);
					vec3 fragmentToLightDir = normalize(fragmentToLight);

					float lightDistance = length(fragmentToLight);

					float constant = 1.0f;
					float llinear = 0.09f;
					float quadratic = 0.032f;
					//float attenuation = 1.0 / (constant + llinear * lightDistance + quadratic * (lightDistance * lightDistance));    
					float attenuation = 1.0 / (lightDistance * lightDistance);
			
					vec3 halfwayVector = normalize(fragmentToLightDir + fragmentToCamera);
					vec3 radiance = GetLightColor(lightIndex) * attenuation;        
        

					vec3 F0 = vec3(0.04); 
					F0      = mix(F0, g_Material.m_DiffuseColor, g_Material.m_Metallic);
					vec3 F  = fresnelSchlick(max(dot(halfwayVector, fragmentToCamera), 0.0), F0);
					float NDF = DistributionGGX(pixelData.m_Normal, halfwayVector, g_Material.m_Roughness);       
					float G   = GeometrySmith(pixelData.m_Normal, fragmentToCamera, fragmentToLightDir, g_Material.m_Roughness);  

					vec3 nominator    = NDF * G * F;
					float denominator = 4 * max(dot(pixelData.m_Normal, fragmentToCamera), 0.0) * max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0) + 0.001; 
					vec3 specular     = nominator / denominator;  

					vec3 kS = F;
					vec3 kD = vec3(1.0) - kS;
					kD *= 1.0 - g_Material.m_Metallic;	  
        
					// add to outgoing radiance Lo
					float NdotL = max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0);                
					FinalColor.rgb += (kD * GetDiffuseColor(myTexCoord).rgb / PI + specular) * radiance * NdotL; 
					//FinalColor.rgb = NdotL * vec3(1.0); 
			}
		}
#endif


		vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
		vec3 sunDir = cos(sunTime)*vec3(0, 0, 1) + sin(sunTime)*vec3(sunDirFlat.x, sunDirFlat.y, 0);

		//vec3 skyLighting = vec3(0, 0, 1);
		float sky = dot(sunDir, pixelData.m_Normal);
		pixelData.m_DiffuseColor = GetDiffuseColor(myTexCoord);

		float shadow = 1.f - ShadowCalculation(FragPosLightSpace, pixelData.m_Normal, gl_FragCoord.xy);

		pixelData.m_FinalColor = pixelData.m_DiffuseColor * sky * shadow;


		//float ShadowCalculation(vec4 fragPosLightSpace, vec3 normalWS, vec2 fragCoord)
		//{
		//	if (!g_UseShadows)
		//	{
		//		return 0.f;
		//	}
		//	// perform perspective divide
		//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
		//	projCoords = projCoords * 0.5 + 0.5;


		//	float closestDepth = texture(g_DepthMapTexture, projCoords.xy).r;

		//	float currentDepth = projCoords.z;
		//	DebugWatch(fragCoord.xy, 0, closestDepth);

		//	float bias = max(0.05 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.005);
		//	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
		//	//if(projCoords.z > 1.0)
		//	//      shadow = 0.0;
		//	return shadow;
		//};

		vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
		//projCoords = projCoords * 0.5 + 0.5;
		////projCoords.x = 1.f - projCoords.x;
		//projCoords.xy *= g_DebugVec4.xy;

		float closestDepth = textureLod(g_DepthMapTexture, projCoords.xy, 0).r;
		//float bias = max(0.05 * (1.0 - dot(g_DirectionalLight.m_Direction, pixelData.m_Normal)), 0.005);
		//float currentDepth = projCoords.z;

		//pixelData.m_FinalColor = vec4(pow(currentDepth, 1000.f).xxx, 1.f);
		//pixelData.m_FinalColor = vec4(closestDepth.xxx, 1.f) ;

		// sky tests
		//float ratio = 1.00 / 1.52;
		//vec3 R = vec3(0.f);

		//if (g_Material.m_ReflectEnvironment)
		//{
		//	R = reflect(-fragmentToCamera, pixelData.m_Normal);
		//	vec3 skyColor = texture(g_Skybox, R).rgb;

		//	// todo handle this blend properly wow
		//	float ratio = g_Material.m_Shininess / 500.f;
		//	FinalColor.rgb = ratio * skyColor + (1.f - ratio) * FinalColor.rgb;
		//}
		
		//if (g_Material.m_IsGlass)
		//{
		//	R = refract(-fragmentToCamera, normalize(pixelData.m_Normal), g_Material.m_RefractionIndex);
		//	vec3 skyColor = texture(g_Skybox, R).rgb;
		//	FinalColor = vec4(skyColor, 1.0);

		//}

	}
	else
	{
		pixelData.m_DiffuseColor = GetDiffuseColor(myTexCoord);
		pixelData.m_FinalColor = pixelData.m_DiffuseColor;
	}

	BrightColor = pixelData.m_FinalColor * g_Material.m_EmissiveFactor;

	if (g_GammaCorrectionEnabled > 0)
	{
		pixelData.m_FinalColor.rgb = pow(pixelData.m_FinalColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
		BrightColor.rgb = pow(BrightColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
	}
	
	FinalColor.rgb = BuildShaderOutput(pixelData);
}