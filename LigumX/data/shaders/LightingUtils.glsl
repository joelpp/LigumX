#ifndef GLSL_LIGHTING_UTILS
#define GLSL_LIGHTING_UTILS

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


vec3 ShadeLight(int lightIndex, Material material, vec3 worldPosition, vec3 worldNormal, vec2 uv, vec3 fragmentToCamera)
{
	vec3 fragmentToLight = GetDirectionToLight(lightIndex, worldPosition.xyz);
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
	F0 = mix(F0, material.m_DiffuseColor, material.m_Metallic);
	vec3 F = fresnelSchlick(max(dot(halfwayVector, fragmentToCamera), 0.0), F0);
	float NDF = DistributionGGX(worldNormal, halfwayVector, material.m_Roughness);
	float G = GeometrySmith(worldNormal, fragmentToCamera, fragmentToLightDir, material.m_Roughness);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(worldNormal, fragmentToCamera), 0.0) * max(dot(worldNormal, fragmentToLightDir), 0.0) + 0.001;
	vec3 specular = nominator / max(denominator, 0.001);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - material.m_Metallic;

	radiance *= 1e2;

	// add to outgoing radiance Lo
	vec4 materialDiffuseColor = GetDiffuseColor(uv);
	float NdotL = max(dot(worldNormal, fragmentToLightDir), 0.0);

	vec3 diffuse = kD * materialDiffuseColor.rgb;
	
	vec3 finalColor = vec3(0.f);
	if (g_EnableDiffuseComponent)
	{
		finalColor += diffuse * radiance * NdotL;
	}
	if (g_EnableSpecularComponent)
	{
		finalColor += specular * radiance * NdotL;
	}

	return finalColor;

}



#endif // GLSL_LIGHTING_UTILS
