#ifndef GLSL_SHADOW_UTILS
#define GLSL_SHADOW_UTILS

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normalWS, vec2 fragCoord)
{
	if (!g_UseShadows)
	{
		return 0.f;
	}
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//projCoords.xy *= g_DebugVec4.xy;
    projCoords = projCoords * 0.5 + 0.5; 


	float closestDepth = textureLod(g_DepthMapTexture, projCoords.xy, 0.f).r;

	float currentDepth = projCoords.z;

	float bias = max(0.01 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.005);
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
		if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

#endif // GLSL_SHADOW_UTILS