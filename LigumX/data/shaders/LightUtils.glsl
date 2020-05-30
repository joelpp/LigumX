#ifndef GLSL_LIGHT_UTILS
#define GLSL_LIGHT_UTILS

vec3 GetLightColor(int lightIndex)
{
	vec3 lightColor;
	if (false && g_UseSkyLighting)
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
	if (false && g_UseSkyLighting)
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


#endif