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

// Include BasicUtils.glsl
// Include ShadowUtils.glsl
// Include LightUtils.glsl
// Include MaterialUtils.glsl
// Include LightingUtils.glsl
// Include SkyUtils.glsl

uniform vec4 g_DebugVec4;

#if DEFERRED
layout(location = 0) out vec4 Normal;
layout(location = 1) out vec4 WorldPosition;
#else
layout(location = 0) out vec4 FinalColor;
layout(location = 1) out vec4 BrightColor;
#endif

bool OutputDebugColors(out vec4 FinalColor, in vec3 screenCoords, in vec2 uvs)
{
	if (g_DebugUVsEnabled > 0)
	{
		vec2 fractUV = fract(uvs);
		FinalColor = vec4(fractUV.r, fractUV.g, 0.f, 1.f);
		return true;
	}

	if (g_DebugDepthEnabled > 0)
	{
		float depth = 0;
		if (g_DebugLinearizeDepth > 0)
		{
			depth = LinearizeDepth(screenCoords.z) / g_CameraFarPlane;
		}
		else
		{
			depth = gl_FragCoord.z;
		}
		FinalColor = vec4(depth, depth, depth, 1.f);
		return true;
	}

	return false;
}



void main() 
{
	PixelData pixelData;

	vec3 screenCoords = gl_FragCoord.xyz;
	float linearDepth = LinearizeDepth(screenCoords.z);

	pixelData.m_UVs = myTexCoord;
	pixelData.m_FinalColor.rgb = vec3(0,0,0);
	pixelData.m_Normal = normalize(vNormalWS);
	pixelData.m_Depth = linearDepth / g_CameraFarPlane;

	vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);

	if (OutputDebugColors(pixelData.m_FinalColor, screenCoords, pixelData.m_UVs))
	{
		return;
	}

	pixelData.m_FinalColor = vec4(0.f, 0.f, 0.f, 1.f);

	// todo : when i do proper attenuation i need to re-visit gamma

	if (g_UseLighting > 0)
	{
		for (int lightIndex = 0; lightIndex < g_NumLights; ++lightIndex)
		{
			pixelData.m_FinalColor.rgb += ShadeLight(lightIndex, g_Material, vWorldPosition.xyz, pixelData.m_Normal, myTexCoord, fragmentToCamera);
		}

		vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
		vec3 sunDir = cos(sunTime)*vec3(0, 0, 1) + sin(sunTime)*vec3(sunDirFlat.x, sunDirFlat.y, 0);

		//vec3 skyLighting = vec3(0, 0, 1);
		float sky = dot(sunDir, pixelData.m_Normal) * 0.1f;
		pixelData.m_DiffuseColor = GetDiffuseColor(myTexCoord);

		float shadow = 1.f - ShadowCalculation(FragPosLightSpace, pixelData.m_Normal, gl_FragCoord.xy);

		if (false)
		{
			pixelData.m_FinalColor += pixelData.m_DiffuseColor * sky * shadow * 0.1f;

		}
		else
		{

			vec3 fragmentToLight = sunDir;
			vec3 fragmentToLightDir = normalize(fragmentToLight);

			float constant = 1.0f;
			float llinear = 0.09f;
			float quadratic = 0.032f;
			//float attenuation = 1.0 / (constant + llinear * lightDistance + quadratic * (lightDistance * lightDistance));    
			//float attenuation = 1.0 / (lightDistance * lightDistance);
			float attenuation = 1.f;

			vec3 halfwayVector = normalize(fragmentToLightDir + fragmentToCamera);
			vec3 radiance = sky * vec3(1, 1, 1)/* GetLightColor(lightIndex)*/ * attenuation;


			vec3 F0 = vec3(0.04);
			F0 = mix(F0, g_Material.m_DiffuseColor, g_Material.m_Metallic);
			vec3 F = fresnelSchlick(max(dot(halfwayVector, fragmentToCamera), 0.0), F0);
			float NDF = DistributionGGX(pixelData.m_Normal, halfwayVector, g_Material.m_Roughness);
			float G = GeometrySmith(pixelData.m_Normal, fragmentToCamera, fragmentToLightDir, g_Material.m_Roughness);

			vec3 nominator = NDF * G * F;
			float denominator = 4 * max(dot(pixelData.m_Normal, fragmentToCamera), 0.0) * max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0) + 0.001;
			vec3 specular = nominator / max(denominator, 0.001);

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - g_Material.m_Metallic;

			radiance *= 1e2;

			// add to outgoing radiance Lo
			float NdotL = max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0);
			vec3 finalColor = (kD * GetDiffuseColor(myTexCoord).rgb / PI + specular) * shadow * radiance * NdotL;
			pixelData.m_FinalColor.rgb += finalColor;
		}

		float ambient = 0.01f;

		if (g_Material.m_Unlit)
		{
			pixelData.m_FinalColor = GetDiffuseColor(myTexCoord);
		}

		//pixelData.m_FinalColor += pixelData.m_DiffuseColor * ambient;

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
		float ratio = 1.00 / 1.52;
		vec3 R = vec3(0.f);

		if (g_Material.m_ReflectEnvironment)
		{
			R = reflect(-fragmentToCamera, pixelData.m_Normal);
			vec3 skyColor = GetSkyColor(R, sunTime, sunOrientation) * 0.1f;
			
			// todo handle this blend properly wow
			float ratio = g_Material.m_Shininess;

			pixelData.m_FinalColor.rgb = ratio * skyColor + (1.f - ratio) * pixelData.m_FinalColor.rgb;
		}
		
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
		pixelData.m_FinalColor.rgb = pixelData.m_FinalColor.rgb / (pixelData.m_FinalColor.rgb + vec3(1.f, 1.f, 1.f));
		pixelData.m_FinalColor.rgb = pow(pixelData.m_FinalColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
		BrightColor.rgb = pow(BrightColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
	}
	
	FinalColor.rgb = BuildShaderOutput(pixelData);
}