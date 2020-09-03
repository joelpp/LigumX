#include "ShadowParameters.h"

#pragma region  CLASS_SOURCE ShadowParameters

#include "ShadowParameters.h"
#include "serializer.h"
const ClassPropertyData ShadowParameters::g_Properties[] = 
{
{ "CascadeDistances", PIDX_CascadeDistances, offsetof(ShadowParameters, m_CascadeDistances), 0, LXType_glmvec4, sizeof(glm::vec4), LXType_glmvec4, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "FirstCascadeToRender", PIDX_FirstCascadeToRender, offsetof(ShadowParameters, m_FirstCascadeToRender), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "LastCascadeToRender", PIDX_LastCascadeToRender, offsetof(ShadowParameters, m_LastCascadeToRender), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "LookAtTarget", PIDX_LookAtTarget, offsetof(ShadowParameters, m_LookAtTarget), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
void ShadowParameters::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVec4(g_Properties[PIDX_CascadeDistances], m_CascadeDistances);
	serializer.SerializeInt(g_Properties[PIDX_FirstCascadeToRender], m_FirstCascadeToRender);
	serializer.SerializeInt(g_Properties[PIDX_LastCascadeToRender], m_LastCascadeToRender);
	serializer.SerializeVec3(g_Properties[PIDX_LookAtTarget], m_LookAtTarget);
}
bool ShadowParameters::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool ShadowParameters::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CascadeDistances], &m_CascadeDistances , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FirstCascadeToRender], &m_FirstCascadeToRender , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_LastCascadeToRender], &m_LastCascadeToRender , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_LookAtTarget], &m_LookAtTarget , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
void ShadowParameters::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	ShadowParameters* other = (ShadowParameters*) otherObj;
	other->SetCascadeDistances(m_CascadeDistances);
	other->SetFirstCascadeToRender(m_FirstCascadeToRender);
	other->SetLastCascadeToRender(m_LastCascadeToRender);
	other->SetLookAtTarget(m_LookAtTarget);
}
const char* ShadowParameters::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE ShadowParameters
