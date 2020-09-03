#ifndef GLSL_SHADOW_UTILS
#define GLSL_SHADOW_UTILS

#define EPSILON 0.00001
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normalWS, vec2 fragCoord)
{
	if (!g_EnableSunShadow)
	{
		return 0.f;
	}
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

#define PCF 1

#if PCF
    vec2 UVCoords;
    UVCoords.x = 0.5 * projCoords.x + 0.5;
    UVCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    ivec2 shadowMapSize = textureSize(g_DepthMapTexture, 0);
    float xOffset = 1.0 / 2048.f;//float(shadowMapSize.x);
    float yOffset = 1.0 / 2048.f;//float(shadowMapSize.y);

    float Factor = 0.0;

    ivec2 start = ivec2(-1, -1);
    ivec2 end = ivec2(1, 1);

    int num = 9;// max(1, (end.x - start.x) * (end.y - start.y));

    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            vec3 UVC = vec3(UVCoords + Offsets, z + EPSILON);
            Factor += texture(g_DepthMapTexture, UVC).r;
        }
    }

    return /*1.f -*/ (Factor / float(num));
#else
	//projCoords.xy *= g_DebugVec4.xy;
    projCoords = projCoords * 0.5 + 0.5; 


	float closestDepth = textureLod(g_DepthMapTexture, projCoords.xy, 0.f).r;

	float currentDepth = projCoords.z;

	float bias = max(0.001 * (1.0 - dot(g_DirectionalLight.m_Direction, normalWS)), 0.001);
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
		if(projCoords.z > 1.0)
        shadow = 0.0;
    return 1.f - shadow;
#endif
}

#endif // GLSL_SHADOW_UTILS