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
	serializer.SerializeBool("CullEntities", m_CullEntities);
	serializer.SerializeBool("CullSectors", m_CullSectors);
	serializer.SerializeBool("UseDotProduct", m_UseDotProduct);
	serializer.SerializeBool("UseAABBClipPos", m_UseAABBClipPos);
	serializer.SerializeBool("DebugAABBClippPos", m_DebugAABBClippPos);
	serializer.SerializeBool("DebugDotProduct", m_DebugDotProduct);
	serializer.SerializeFloat("DebugDotProductMaxDistance", m_DebugDotProductMaxDistance);
}
bool CullingOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool CullingOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("CullEntities", m_CullEntities  );
	ImguiHelpers::ShowBool("CullSectors", m_CullSectors  );
	ImguiHelpers::ShowBool("UseDotProduct", m_UseDotProduct  );
	ImguiHelpers::ShowBool("UseAABBClipPos", m_UseAABBClipPos  );
	ImguiHelpers::ShowBool("DebugAABBClippPos", m_DebugAABBClippPos  );
	ImguiHelpers::ShowBool("DebugDotProduct", m_DebugDotProduct  );
	ImguiHelpers::ShowFloat("DebugDotProductMaxDistance", m_DebugDotProductMaxDistance , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
const char* CullingOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE CullingOptions
