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

vec3 GetLightPosition(int lightIndex)
{
	return g_PointLight[lightIndex].m_Position;
}

vec3 GetDirectionToLight(int lightIndex, vec3 worldPosition)
{
	vec3 lightDirection = GetLightPosition(lightIndex) - worldPosition.xyz;
	return lightDirection;
}


#endif