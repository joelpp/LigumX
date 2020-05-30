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

#endif // GLSL_LIGHTING_UTILS
