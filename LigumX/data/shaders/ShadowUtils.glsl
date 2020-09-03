#ifndef GLSL_SHADOW_UTILS
#define GLSL_SHADOW_UTILS

#define EPSILON 0.00001

bool OutsideClipSpace(vec3 clipCoords)
{
    return (any(lessThan(clipCoords, vec3(-1, -1, -1))) || any(greaterThan(clipCoords, vec3(1, 1, 1))));
}


float ShadowCalculation(sampler2DShadow shadowMapTexture, vec3 projCoords, vec3 normalWS, vec2 fragCoord, vec3 sunDirection)
{	// perform perspective divide

#define PCF 1

#if PCF
    vec2 UVCoords;
    UVCoords.x = 0.5 * projCoords.x + 0.5;
    UVCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    ivec2 shadowMapSize = textureSize(shadowMapTexture, 0);
    float xOffset = 1.0 / float(shadowMapSize.x);
    float yOffset = 1.0 / float(shadowMapSize.y);

    ivec2 start = ivec2(-1, -1);
    ivec2 end = ivec2(1, 1);

    float cosTheta = dot(normalWS, sunDirection);
    float bias = 0.005 * tan(acos(cosTheta));
    bias = clamp(bias, 0, 0.01f);

    float Factor = 0.0;
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            vec3 UVC = vec3(UVCoords + Offsets, z - bias);
            Factor += texture(shadowMapTexture, UVC).r;
        }
    }

    return (Factor / 9.f);
#else
    //projCoords.xy *= g_DebugVec4.xy;
    projCoords = projCoords * 0.5 + 0.5;


    float closestDepth = textureLod(g_ShadowCascade0, projCoords.xy, 0.f).r;

    float currentDepth = projCoords.z;

    float bias = max(0.001 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.001);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return 1.f - shadow;
#endif

}

float GetCascade(vec4 fragPosCascade0, vec4 fragPosCascade1, vec4 fragPosCascade2, vec4 fragPosCascade3,
                 vec3 normalWS, vec2 fragCoord, vec3 sunDirection)
{
    vec3 projCoords0 = fragPosCascade0.xyz / fragPosCascade0.w;

    if (OutsideClipSpace(projCoords0))
    {
        vec3 projCoords1 = fragPosCascade1.xyz / fragPosCascade1.w;
        if (OutsideClipSpace(projCoords1))
        {
            vec3 projCoords2 = fragPosCascade2.xyz / fragPosCascade2.w;
            if (OutsideClipSpace(projCoords2))
            {
                vec3 projCoords3 = fragPosCascade3.xyz / fragPosCascade3.w;
                return ShadowCalculation(g_ShadowCascade3, projCoords3, normalWS, fragCoord, sunDirection);
            }
            else
            {
                return ShadowCalculation(g_ShadowCascade2, projCoords2, normalWS, fragCoord, sunDirection);
            }
        }
        else
        {
            return ShadowCalculation(g_ShadowCascade1, projCoords1, normalWS, fragCoord, sunDirection);
        }
    }
    else
    {
        return ShadowCalculation(g_ShadowCascade0, projCoords0, normalWS, fragCoord, sunDirection);
    }

}



float ShadowCalculation(vec4 fragPosCascade0, vec4 fragPosCascade1, vec4 fragPosCascade2, vec4 fragPosCascade3, 
                        vec3 normalWS, vec2 fragCoord, vec3 sunDirection)
{
	if (!g_EnableSunShadow)
	{
		return 0.f;
	}

    return GetCascade(fragPosCascade0, fragPosCascade1, fragPosCascade2, fragPosCascade3, normalWS, fragCoord, sunDirection);
    //return 0.5f * cascade;

    //vec3 projCoords = fragPosCascade0.xyz / fragPosCascade0.w;
    //return ShadowCalculation(g_ShadowCascade0, projCoords, normalWS, fragCoord, sunDirection);

}

#endif // GLSL_SHADOW_UTILS