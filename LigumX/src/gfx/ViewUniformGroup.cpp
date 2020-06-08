#include "ViewUniformGroup.h"

ViewUniformGroup::ViewUniformGroup()
{
	SetGroupName("View");

	AddUniform(GFXShaderStage_Vertex, "g_WorldToViewMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Vertex, "g_WorldToViewMatrixNoTranslation", LXType_glmmat4);
	AddUniform(GFXShaderStage_Vertex, "g_WorldToViewMatrixRotationOnly", LXType_glmmat4);
	AddUniform(GFXShaderStage_Vertex, "g_ProjectionMatrix", LXType_glmmat4);

	AddUniform(GFXShaderStage_Fragment, "g_ViewToWorldMatrix", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_ViewProjectionMatrixInverse", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_ViewMatrixInverse", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_ProjectionMatrixInverse", LXType_glmmat4);
	AddUniform(GFXShaderStage_Fragment, "g_CameraPosition", LXType_glmvec3);
	AddUniform(GFXShaderStage_Fragment, "g_CameraNearPlane", LXType_glmvec3);
	AddUniform(GFXShaderStage_Fragment, "g_CameraFarPlane", LXType_glmvec3);
	AddUniform(GFXShaderStage_Fragment, "g_CameraLookAt", LXType_glmvec3);

}
