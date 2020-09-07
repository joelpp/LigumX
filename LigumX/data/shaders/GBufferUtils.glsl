
#ifdef DEFERRED_RESOLVE // read g buffer utils


#else // write gbuffer utils
void GBuffer_WriteNormal(vec3 normal)
{
	outGBuffer0.rgb = EncodeNormal(normal);
}

void GBuffer_WriteMetallic(float metallic)
{
	outGBuffer0.a = metallic;
}


void GBuffer_WriteWorldPosition(vec3 worldPosition)
{
	outGBuffer1.rgb = worldPosition.xyz;
}


void GBuffer_WriteRoughness(float roughness)
{
	outGBuffer1.a = roughness;
}


void GBuffer_WriteAlbedo(vec3 albedo)
{
	outGBuffer2.rgb = albedo.rgb;
}


#endif // DEFERRED_RESOLVE