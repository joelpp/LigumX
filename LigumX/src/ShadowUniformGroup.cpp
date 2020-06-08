#include "ShadowUniformGroup.h"

ShadowUniformGroup::ShadowUniformGroup()
{
	SetGroupName("ShadowMap");

	{
		GFXUniformDescription desc;
		desc.SetUniformName("g_LightProjectionMatrix");
		desc.SetType(LXType_glmmat4);
		GetUniformMap(GFXShaderStage_Vertex).emplace(desc.GetUniformName(), desc);
	}
	{
		GFXUniformDescription desc;
		desc.SetUniformName("g_DepthMapTexture");
		desc.SetType(LXType_Texture);
		GetUniformMap(GFXShaderStage_Fragment).emplace(desc.GetUniformName(), desc);
	}


}
