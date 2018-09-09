#include "CullingOptions.h"

#pragma region  CLASS_SOURCE CullingOptions

#include "CullingOptions.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData CullingOptions::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(CullingOptions, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(CullingOptions, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0, }, 
{ "CullEntities", PIDX_CullEntities, offsetof(CullingOptions, m_CullEntities), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "CullSectors", PIDX_CullSectors, offsetof(CullingOptions, m_CullSectors), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "UseDotProduct", PIDX_UseDotProduct, offsetof(CullingOptions, m_UseDotProduct), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "UseAABBClipPos", PIDX_UseAABBClipPos, offsetof(CullingOptions, m_UseAABBClipPos), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "DebugAABBClippPos", PIDX_DebugAABBClippPos, offsetof(CullingOptions, m_DebugAABBClippPos), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "DebugDotProduct", PIDX_DebugDotProduct, offsetof(CullingOptions, m_DebugDotProduct), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0, }, 
{ "DebugDotProductMaxDistance", PIDX_DebugDotProductMaxDistance, offsetof(CullingOptions, m_DebugDotProductMaxDistance), 0, LXType_float, false, LXType_None, 0, 0, 0, 0, }, 
};
bool CullingOptions::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE CullingOptions
