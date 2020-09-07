#version 440 core

#define DEFERRED_RESOLVE

in vec4 gl_FragCoord;
in vec2 uv;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D g_GBuffer0;
layout(binding = 1) uniform sampler2D g_GBuffer1;
layout(binding = 2) uniform sampler2D g_GBuffer2;
layout(binding = 3) uniform sampler2D g_GBuffer3;
layout(binding = 4) uniform sampler2D g_GBufferDepth;
layout(binding = 6) uniform sampler2D g_SSAOTexture;
//in vec4 FragPosCascade0;
//in vec4 FragPosCascade1;
//in vec4 FragPosCascade2;
//in vec4 FragPosCascade3;

#define PROVIDER_View
#define PROVIDER_ShadowMap
#define PROVIDER_Sky
#define PROVIDER_DisplayOptions
#define PROVIDER_Debug
#define PROVIDER_LightingOptions
#define PROVIDER_Light
#define PROVIDER_Material
#define PROVIDER_PostEffects

// Include ProvidersMarker


// Include BasicUtils.glsl
// Include ShadowUtils.glsl
// Include LightUtils.glsl
// Include MaterialUtils.glsl
// Include LightingUtils.glsl
// Include SkyUtils.glsl




void main() 
{
	vec4 gBuffer0 = SampleTexture(g_GBuffer0, uv);
	vec4 gBuffer1 = SampleTexture(g_GBuffer1, uv);
	vec4 gBuffer2 = SampleTexture(g_GBuffer2, uv);
	vec4 gBuffer3 = SampleTexture(g_GBuffer3, uv);
	float gBufferDepth = SampleTexture(g_GBufferDepth, uv).r;
	
	// todo jpp : should be done with stencil maybe?
	if (length(gBuffer0) == 0)
	{
		discard;
	}

	vec3 wsNormal = (gBuffer0.rgb * 2.f) - vec3(1.f);

	float metallic = gBuffer0.a;
	float roughness = gBuffer1.a;
	vec3 worldPosition = gBuffer1.rgb;
	vec3 materialAlbedo = gBuffer2.rgb;

	vec3 fragmentToCamera = normalize(g_CameraPosition - worldPosition.xyz);

	FragColor.rgb = vec3(0.f);

	if (g_UseLighting > 0)
	{
		//if (g_EnableDynamicLights)
		//{
		//	for (int lightIndex = 0; lightIndex < g_NumLights; ++lightIndex)
		//	{
		//		pixelData.m_FinalColor.rgb += ShadeLight(lightIndex, g_Material, vWorldPosition.xyz, pixelData.m_Normal, pixelData.m_UVs, fragmentToCamera);
		//	}
		//}

		vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
		vec3 sunDir = cos(sunTime) * vec3(0, 0, 1) + sin(sunTime) * vec3(sunDirFlat.x, sunDirFlat.y, 0);

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

			float sky = dot(sunDir, wsNormal) * 0.1f;

			// Turn sun off at night
			float nightFactor = saturate(sin(sunTime - (3.f * PI / 2.f)));

			vec3 radiance = nightFactor * sky * vec3(1, 1, 1)/* GetLightColor(lightIndex)*/ * attenuation;

			vec3 F0 = vec3(0.04);
			F0 = mix(F0, materialAlbedo.rgb, metallic);
			vec3 F = fresnelSchlick(max(dot(halfwayVector, fragmentToCamera), 0.0), F0);
			float NDF = DistributionGGX(wsNormal, halfwayVector, roughness);
			float G = GeometrySmith(wsNormal, fragmentToCamera, fragmentToLightDir, roughness);

			vec3 nominator = NDF * G * F;
			float denominator = 4 * max(dot(wsNormal, fragmentToCamera), 0.0) * max(dot(wsNormal, fragmentToLightDir), 0.0) + 0.001;
			vec3 specular = nominator / max(denominator, 0.001);

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;

			radiance *= 1e2;

			// add to outgoing radiance Lo
			float NdotL = max(dot(wsNormal, fragmentToLightDir), 0.0);

			vec4 FragPosCascade0 = g_Cascade0ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
			vec4 FragPosCascade1 = g_Cascade1ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
			vec4 FragPosCascade2 = g_Cascade2ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
			vec4 FragPosCascade3 = g_Cascade3ProjectionMatrix * vec4(worldPosition.xyz, 1.f);
			float shadow = ShadowCalculation(FragPosCascade0, FragPosCascade1, FragPosCascade2, FragPosCascade3, wsNormal, gl_FragCoord.xy, sunDir);
			vec3 finalColor = (kD * materialAlbedo.rgb / PI + specular) * shadow * radiance * NdotL;

			FragColor.rgb += finalColor;
		}
	}

	if (g_EnableAmbientLighting)
	{
		vec3 skyColor = vec3(0.52, 0.75, 0.88);

		vec3 up = vec3(0, 0, 1);

		float upFactor = dot(up, wsNormal); // [-1 down, 1 up]
		upFactor = saturate(0.5f * (1.f + upFactor)); // 0 bottom hemisphere, 1 up

		FragColor.rgb += (skyColor * upFactor * g_AmbientLightingGlobalFactor * materialAlbedo.rgb);
	}

	vec4 ssao = SampleTexture(g_SSAOTexture, uv);
	FragColor.rgb *= ssao.rgb;

	if (g_GammaCorrectionEnabled > 0)
	{
		FragColor.rgb = FragColor.rgb / (FragColor.rgb + vec3(1.f, 1.f, 1.f));
		FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
	}


	gl_FragDepth = gBufferDepth;
}

