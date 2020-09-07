#include "ShadowUniformGroup.h"

ShadowUniformGroup::ShadowUniformGroup()
{
	SetUniformGroupType(UniformGroupType_ShadowMap);

	AddUniform(GFXShaderStage_Fragment, "g_Cascade0ProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_Cascade1ProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_Cascade2ProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_Cascade3ProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_ShadowCascade0", LXType_Texture);
	AddUniform(GFXShaderStage_Fragment, "g_ShadowCascade1", LXType_Texture);
	AddUniform(GFXShaderStage_Fragment, "g_ShadowCascade2", LXType_Texture);
	AddUniform(GFXShaderStage_Fragment, "g_ShadowCascade3", LXType_Texture);
}
