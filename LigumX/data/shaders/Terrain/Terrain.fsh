#version 430 core
in float v_Height;
in vec2 v_TexCoords;
in vec3 v_Normal;
in float v_maxHeight;
in vec4 FragPosCascade0;
in vec4 FragPosCascade1;
in vec4 FragPosCascade2;
in vec4 FragPosCascade3;
in vec3 vWorldPosition;

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


layout(binding = 3) uniform sampler2D g_HeightfieldTexture;
layout(binding = 1) uniform sampler2D g_SplatMapTexture;


layout(binding = 4) uniform sampler2D g_DirtTexture;
layout(binding = 5) uniform sampler2D g_SandTexture;
layout(binding = 6) uniform sampler2D g_WoodTexture;
layout(binding = 7) uniform sampler2D g_AlbedoTexture;



vec3 ComputeNormal(float heightMid, vec2 texCoord, float resolution)
{
	vec2 du = vec2(resolution, 0);
	vec2 dv = vec2(0, resolution);

	float heightLeft = textureLod(g_HeightfieldTexture, texCoord - du, 0.f).r;

	float heightRight =	textureLod(g_HeightfieldTexture, texCoord + du, 0.f).r;

	float heightBottom = textureLod(g_HeightfieldTexture, texCoord - dv , 0.f).r;
	float heightTop = textureLod(g_HeightfieldTexture, texCoord + dv , 0.f).r;

	float dhdu = (2 * resolution) * (heightRight - heightLeft); 
	float dhdv = (2 * resolution) * (heightTop - heightBottom); 

	//vec3 normal = normalize(vec3(0,0,1) + vec3(1,0,0) * dhdu + vec3(0,1,0) * dhdv);

	vec3 va = normalize(vec3(2, 0, 1.f / 25 * (heightRight - heightLeft)));
    vec3 vb = normalize(vec3(0, 2, 1.f / 25 * (heightTop - heightBottom)));
    vec3 normal = cross(va,vb);

	return normalize(normal);
}
vec3 blend(vec3 texture1, float a1, vec3 texture2, float a2)
{
    float depth = 0.2;
    float ma = max(a1, a2) - depth;

    float b1 = max(a1 - ma, 0);
    float b2 = max(a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}

out vec4 o_PSOutput;
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
			depth = LinearizeDepth(screenCoords.z, g_CameraNearPlane, g_CameraFarPlane) / g_CameraFarPlane;
		}
		else
		{
			depth = gl_FragCoord.z;
		}
		o_PSOutput = vec4(depth, depth, depth, 1.f);
		return true;
	}

	if (g_DebugNormalsEnabled > 0)
	{
		o_PSOutput = GetDebugNormalColor(pixelData.m_Normal);
		return true;
	}
	return false;
}


void main() 
{
	PixelData pixelData;

	pixelData.m_UVs = v_TexCoords;
	pixelData.m_Depth = LinearizeDepth(gl_FragCoord.z, g_CameraNearPlane, g_CameraFarPlane) / g_CameraFarPlane;

	float resolution = 1.f / 64;
	pixelData.m_Normal = ComputeNormal(v_Height, pixelData.m_UVs, resolution);
	
	vec2 splatTexCoords = vec2(1 - v_TexCoords.x, v_TexCoords.y);
	vec4 splatMap = textureLod(g_SplatMapTexture, v_TexCoords, 0.0f);
	//vec4 splatMap = texture(g_SplatMapTexture, v_TexCoords);

	//vec3 lightDirection = normalize(vec3(0.5f, 0.5f, 1.0f));
	vec3 screenCoords = gl_FragCoord.xyz;
	if (OutputDebugColors(pixelData, screenCoords))
	{
		return;
	}

	vec3 lightDirection = normalize(vec3(0.f, 0.f, 1.0f));
    float lightPower = dot(pixelData.m_Normal, lightDirection);

	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);

	if ( any(greaterThan( splatMap.rgb, vec3(0,0,0) )) )
	{
		//diffuse = splatMap.rgb;
	} 

	float uvScale = 1.f;
	vec2 diffuseTexCoords = pixelData.m_UVs * uvScale;


	//diffuse = splatMap.x *
	//		+ splatMap.y * texture(g_RockTexture, diffuseTexCoords).rgb
	//		+ splatMap.z * texture(g_SandTexture, diffuseTexCoords).rgb
	//		+ splatMap.w * texture(g_WoodTexture, diffuseTexCoords).rgb;
	
	vec3 grass =  texture(g_Material.m_DiffuseTexture, diffuseTexCoords).rgb;
	vec3 Dirt =  texture(g_DirtTexture, diffuseTexCoords).rgb;
	//vec3 Sand =  texture(g_SandTexture, diffuseTexCoords).rgb;
	//vec3 Wood =  texture(g_WoodTexture, diffuseTexCoords).rgb;
	//vec3 neutral = vec3(0.2, 0.1, 0.3);
	//diffuse = grass * splatMap.x;
	//diffuse += Rock * splatMap.y;
	//diffuse += Sand * splatMap.z;
	//diffuse += Wood * splatMap.w;

	//vec3 lerp0 = mix(neutral, grass, splatMap.x);
	//lerp0 = mix(lerp0, Rock, splatMap.y);
	//lerp0 = mix(lerp0, Sand, splatMap.z);
	pixelData.m_DiffuseColor.rgb = mix(grass, Dirt, 1.f - splatMap.z);
	//pixelData.m_DiffuseColor.rgb = splatMap.rgb * splatMap.a + Rock * (1 - splatMap.a);
	//pixelData.m_DiffuseColor = splatMap.rgba;

	//diffuse = mix(diffuse, grass, splatMap.x);
	//diffuse = mix(diffuse, Rock, splatMap.y);
	//diffuse = mix(diffuse, Sand, splatMap.z);
	//diffuse = mix(diffuse, Wood, splatMap.w);

	//float a = splatMap.x + splatMap.y * (1 - splatMap.x);
	//vec3 c0 = (grass * splatMap.x + Rock * splatMap.y * (1 - splatMap.x)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}

	//float olda = a;
	//a = olda + splatMap.z * (1 - olda);
	//c0 = (c0 * olda + Sand * splatMap.z * (1 - olda)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}
	//olda = a;
	//a = olda + splatMap.w * (1 - olda);
	//c0 = (c0 * olda + Wood * splatMap.w * (1 - olda)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}

	//diffuse *= texture(g_Material.m_DiffuseTexture, diffuseTexCoords).rgb;
	//diffuse = vec3(0.5f * (1.f + v_Height / 300.f), 0, 0);
	//diffuse = c0;
	vec3 fragmentToCamera = normalize(g_CameraPosition - vWorldPosition.xyz);

	pixelData.m_FinalColor = 0.f * pixelData.m_DiffuseColor * lightPower;

	vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
	vec3 sunDir = cos(sunTime) * vec3(0, 0, 1) + sin(sunTime) * vec3(sunDirFlat.x, sunDirFlat.y, 0);

	//vec3 skyLighting = vec3(0, 0, 1);
	vec3 materialAlbedo = GetDiffuseColor(pixelData.m_UVs).rgb;
	pixelData.m_DiffuseColor = vec4(materialAlbedo, 1.f);

	if (g_EnableSunlight)
	{
		//vec2 sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
		//vec3 sunDir = cos(sunTime) * vec3(0, 0, 1) + sin(sunTime) * vec3(sunDirFlat.x, sunDirFlat.y, 0);
		//vec3 fragmentToLightDir = sunDir;

		//float NdotL = max(dot(pixelData.m_Normal, fragmentToLightDir), 0.0);

		//float shadow = 1.f - ShadowCalculation(FragPosLightSpace, pixelData.m_Normal, gl_FragCoord.xy);
		//float nightFactor = saturate(sin(sunTime - (3.f * PI / 2.f)));

		//float sunPower = 1.f;
		//float sunLight = nightFactor * shadow * NdotL * sunPower;
		//pixelData.m_FinalColor += pixelData.m_DiffuseColor * sunLight;

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

		float terrainFactor = 1.f;// todo jpp sort this out
		pixelData.m_FinalColor.rgb += (skyColor * upFactor * g_AmbientLightingGlobalFactor * pixelData.m_DiffuseColor.rgb * terrainFactor);

	}

	//vec3 finalColor = diffuse /** lightPower*/;
	//finalColor *= vec3(v_TexCoords, 0);


	vec4 BrightColor = pixelData.m_FinalColor * g_Material.m_EmissiveFactor;

	if (g_GammaCorrectionEnabled > 0)
	{
		pixelData.m_FinalColor.rgb = pixelData.m_FinalColor.rgb / (pixelData.m_FinalColor.rgb + vec3(1.f, 1.f, 1.f));
		pixelData.m_FinalColor.rgb = pow(pixelData.m_FinalColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
		BrightColor.rgb = pow(BrightColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));
	}
	vec3 shaderOutput = BuildShaderOutput(pixelData);

    o_PSOutput = vec4(shaderOutput, 1.0);

	



    //o_PSOutput = vec4(0.5f * (normal + vec3(1,1,1)), 1.0);
   
}