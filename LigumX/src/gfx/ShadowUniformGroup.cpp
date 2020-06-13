#include "ShadowUniformGroup.h"

ShadowUniformGroup::ShadowUniformGroup()
{
	SetUniformGroupType(UniformGroupType_ShadowMap);

	AddUniform(GFXShaderStage_Vertex, "g_LightProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_DepthMapTexture", LXType_Texture);
}
