#include "SectorGraphicalData.h"
#include "Mesh.h"

#pragma region  CLASS_SOURCE SectorGraphicalData

#include "SectorGraphicalData.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData SectorGraphicalData::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(SectorGraphicalData, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(SectorGraphicalData, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "NodesMesh", PIDX_NodesMesh, offsetof(SectorGraphicalData, m_NodesMesh), 0, LXType_Mesh, true, LXType_None, 0, 0, 0, }, 
{ "WaysMesh", PIDX_WaysMesh, offsetof(SectorGraphicalData, m_WaysMesh), 0, LXType_Mesh, true, LXType_None, 0, 0, 0, }, 
};
bool SectorGraphicalData::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorGraphicalData
