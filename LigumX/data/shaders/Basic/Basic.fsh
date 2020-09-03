#version 430 core

#define DEFERRED 0

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;
in vec4 FragPosCascade0;
in vec4 FragPosCascade1;
in vec4 FragPosCascade2;
in vec4 FragPosCascade3;

#define PROVIDER_View
#define PROVIDER_Light
#define PROVIDER_PostEffects
#define PROVIDER_Material
#define PROVIDER_ShadowMap
#define PROVIDER_Sky
#define PROVIDER_DisplayOptions
#define PROVIDER_Debug
#define PROVIDER_DataInspector
#define PROVIDER_LightingOptions

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

bool OutputDebugColors(inout PixelData pixelData, in vec3 screenCoords)
{
	if (g_DebugUVsEnabled > 0)
	{
		vec2 fractUV = fract(pixelData.m_UVs);
		pixelData.m_FinalColor = vec4(fractUV.r, fractUV.g, 0.f, 1.f);
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

	if (g_DebugNormalsEnabled > 0)
	{
		FinalColor = GetDebugNormalColor(pixelData.m_Normal);
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
	pixelData.m_Normal *= (gl_FrontFacing ? 1 : -1);

	pixelData.m_Depth = linearDepth / g_CameraFarPlane;

	vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);

	if (OutputDebugColors(pixelData, screenCoords))
	{
		return;
	}


	pixelData.m_FinalColor = vec4(0.f, 0.f, 0.f, 1.f);

	// todo : when i do proper attenuation i need to re-visit gamma

	if (g_UseLighting > 0)
	{
		if (g_EnableDynamicLights)
		{
			for (int lightIndex = 0; lightIndex < g_NumLights; ++lightIndex)
			{
				pixelData.m_FinalColor.rgb += ShadeLight(lightIndex, g_Material, vWorldPosition.xyz, pixelData.m_Normal, pixelData.m_UVs, fragmentToCamera);
			}
		}

		vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
		vec3 sunDir = cos(sunTime)*vec3(0, 0, 1) + sin(sunTime)*vec3(sunDirFlat.x, sunDirFlat.y, 0);

		//vec3 skyLighting = vec3(0, 0, 1);
		vec3 materialAlbedo = GetDiffuseColor(pixelData.m_UVs).rgb;
		pixelData.m_DiffuseColor = vec4(materialAlbedo, 1.f);


		if (g_EnableSunlight)
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

			float sky = dot(sunDir, pixelData.m_Normal) * 0.1f;

			// Turn sun off at night
			float nightFactor = saturate(sin(sunTime - (3.f * PI / 2.f)));

			vec3 radiance = nightFactor * sky * vec3(1, 1, 1)/* GetLightColor(lightIndex)*/ * attenuation;


			vec3 F0 = vec3(0.04);
			F0 = mix(F0, materialAlbedo.rgb, GetMetallic(pixelData.m_UVs));
			vec3 F = fresnelSchlick(max(dot(halfwayVector, fragmentToCamera), 0.0), F0);
			float NDF = DistributionGGX(pixelData.m_Normal, halfwayVector, GetRoughness(pixelData.m_UVs));
			float G = GeometrySmith(pixelData.m_Normal, fragmentToCamera, fragmentToLightDir, GetRoughness(pixelData.m_UVs));

			vec3 nominator = NDF * G * F;
			float denominator = 4 * max(dot(pixelData.m_Normal, fragmentToCamera), 0.0) * max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0) + 0.001;
			vec3 specular = nominator / max(denominator, 0.001);

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - GetMetallic(pixelData.m_UVs);

			radiance *= 1e2;

			// add to outgoing radiance Lo
			float NdotL = max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0);
			float shadow = ShadowCalculation(FragPosCascade0, FragPosCascade1, FragPosCascade2, FragPosCascade3, pixelData.m_Normal, gl_FragCoord.xy, sunDir);
			vec3 finalColor = (kD * materialAlbedo.rgb / PI + specular) * shadow * radiance * NdotL;

			pixelData.m_FinalColor.rgb += finalColor;
		}

		if (g_EnableAmbientLighting)
		{

			vec3 skyColor = vec3(0.52, 0.75, 0.88);

			vec3 up = vec3(0, 0, 1);
			
			float upFactor = dot(up, pixelData.m_Normal); // [-1 down, 1 up]
			upFactor = saturate(0.5f * (1.f + upFactor)); // 0 bottom hemisphere, 1 up

			pixelData.m_FinalColor.rgb += (skyColor * upFactor * g_AmbientLightingGlobalFactor * materialAlbedo.rgb);

		}

		if (g_Material.m_Unlit)
		{
			pixelData.m_FinalColor = vec4(materialAlbedo, 1.f);
		}

		// sky tests
		float ratio = 1.00 / 1.52;
		vec3 R = vec3(0.f);

		if (g_Material.m_ReflectEnvironment && g_EnableReflection)
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