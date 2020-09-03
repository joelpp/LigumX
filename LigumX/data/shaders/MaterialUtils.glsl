#ifndef GLSL_MATERIAL_UTILS
#define GLSL_MATERIAL_UTILS

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


float GetRoughness(vec2 uv)
{
	if (g_EnableReplacementRoughness)
	{
		return g_ReplacementRoughness;
	}

	return g_Material.m_Roughness;
}



float GetMetallic(vec2 uv)
{
	if (g_EnableReplacementMetallic)
	{
		return g_ReplacementMetallic;
	}

	return g_Material.m_Metallic;
}


vec4 GetDiffuseColor(vec2 uv)
{
	if (g_EnableReplacementAlbedo)
	{
		return vec4(g_ReplacementAlbedo.xyz, 1.f);
	}

	vec4 diffuseColor = vec4(1.f, 1.f, 1.f, 1.f);
	if (g_Material.m_DiffuseTextureEnabled)
	{
		vec4 textureColor = SampleTexture(g_Material.m_DiffuseTexture, uv * g_Material.m_UVScale);
		diffuseColor *= textureColor;

		if (g_GammaCorrectionEnabled > 0)
		{
			diffuseColor.rgb = pow(diffuseColor.rgb, vec3(g_GammaCorrectionExponent));
		}
	} 

	diffuseColor *= vec4(g_Material.m_DiffuseColor, 1.0f);

	return diffuseColor;
}

#endif // GLSL_MATERIAL_UTILS
