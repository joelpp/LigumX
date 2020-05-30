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

#endif // GLSL_MATERIAL_UTILS
