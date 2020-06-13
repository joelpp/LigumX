#include "LightingOptionsUniformGroup.h"

LightingOptionsUniformGroup::LightingOptionsUniformGroup()
{
	SetUniformGroupType(UniformGroupType_LightingOptions);

	AddUniform(GFXShaderStage_Fragment, "g_EnableDynamicLights", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableSunlight", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableAmbientLighting", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableDiffuseComponent", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableSpecularComponent", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableReflection", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableSunShadow", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableDynamicShadows", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_EnableReplacementAlbedo", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_ReplacementAlbedo", LXType_glmvec3);
	AddUniform(GFXShaderStage_Fragment, "g_EnableReplacementMetallic", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_ReplacementMetallic", LXType_float);
	AddUniform(GFXShaderStage_Fragment, "g_EnableReplacementRoughness", LXType_bool);
	AddUniform(GFXShaderStage_Fragment, "g_ReplacementRoughness", LXType_float);

}
