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
void CullingOptions::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_CullEntities], m_CullEntities);
	serializer.SerializeBool(g_Properties[PIDX_CullSectors], m_CullSectors);
	serializer.SerializeBool(g_Properties[PIDX_UseDotProduct], m_UseDotProduct);
	serializer.SerializeBool(g_Properties[PIDX_UseAABBClipPos], m_UseAABBClipPos);
	serializer.SerializeBool(g_Properties[PIDX_DebugAABBClippPos], m_DebugAABBClippPos);
	serializer.SerializeBool(g_Properties[PIDX_DebugDotProduct], m_DebugDotProduct);
	serializer.SerializeFloat(g_Properties[PIDX_DebugDotProductMaxDistance], m_DebugDotProductMaxDistance);
}
bool CullingOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool CullingOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CullEntities], &m_CullEntities  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CullSectors], &m_CullSectors  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseDotProduct], &m_UseDotProduct  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseAABBClipPos], &m_UseAABBClipPos  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DebugAABBClippPos], &m_DebugAABBClippPos  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DebugDotProduct], &m_DebugDotProduct  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DebugDotProductMaxDistance], &m_DebugDotProductMaxDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
void CullingOptions::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	CullingOptions* other = (CullingOptions*) otherObj;
	other->SetCullEntities(m_CullEntities);
	other->SetCullSectors(m_CullSectors);
	other->SetUseDotProduct(m_UseDotProduct);
	other->SetUseAABBClipPos(m_UseAABBClipPos);
	other->SetDebugAABBClippPos(m_DebugAABBClippPos);
	other->SetDebugDotProduct(m_DebugDotProduct);
	other->SetDebugDotProductMaxDistance(m_DebugDotProductMaxDistance);
}
const char* CullingOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE CullingOptions