#include "ShadowUniformGroup.h"

ShadowUniformGroup::ShadowUniformGroup()
{
	SetGroupName("ShadowMap");
	AddUniform(GFXShaderStage_Vertex, "g_LightProjectionMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_DepthMapTexture", LXType_Texture);
}
