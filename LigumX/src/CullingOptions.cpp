#include "CullingOptions.h"

#pragma region  CLASS_SOURCE CullingOptions

#include "CullingOptions.h"
#include "serializer.h"
const ClassPropertyData CullingOptions::g_Properties[] = 
{
{ "CullEntities", PIDX_CullEntities, offsetof(CullingOptions, m_CullEntities), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "CullSectors", PIDX_CullSectors, offsetof(CullingOptions, m_CullSectors), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "UseDotProduct", PIDX_UseDotProduct, offsetof(CullingOptions, m_UseDotProduct), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "UseAABBClipPos", PIDX_UseAABBClipPos, offsetof(CullingOptions, m_UseAABBClipPos), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugAABBClippPos", PIDX_DebugAABBClippPos, offsetof(CullingOptions, m_DebugAABBClippPos), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugDotProduct", PIDX_DebugDotProduct, offsetof(CullingOptions, m_DebugDotProduct), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugDotProductMaxDistance", PIDX_DebugDotProductMaxDistance, offsetof(CullingOptions, m_DebugDotProductMaxDistance), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
bool CullingOptions::Serialize(Serializer2& serializer)
{
	return true;
}
bool CullingOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool CullingOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("CullEntities", m_CullEntities);
	LXIMGUI_SHOW_BOOL("CullSectors", m_CullSectors);
	LXIMGUI_SHOW_BOOL("UseDotProduct", m_UseDotProduct);
	LXIMGUI_SHOW_BOOL("UseAABBClipPos", m_UseAABBClipPos);
	LXIMGUI_SHOW_BOOL("DebugAABBClippPos", m_DebugAABBClippPos);
	LXIMGUI_SHOW_BOOL("DebugDotProduct", m_DebugDotProduct);
	LXIMGUI_SHOW_FLOAT("DebugDotProductMaxDistance", m_DebugDotProductMaxDistance, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	return true;
}
const char* CullingOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE CullingOptions
