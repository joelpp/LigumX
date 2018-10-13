#include "OSMElementTypeData.h"

#pragma region  CLASS_SOURCE OSMElementTypeData

#include "OSMElementTypeData.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMElementTypeData::g_Properties[] = 
{
{ "Attributes", PIDX_Attributes, offsetof(OSMElementTypeData, m_Attributes), 0, LXType_stdvector, sizeof(std::vector<OSMAttribute*>), LXType_stdvector, false, LXType_OSMAttribute, 0, 0, 0, 0,}, 
};
bool OSMElementTypeData::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMElementTypeData
