#include "OSMAttribute.h"

#pragma region  CLASS_SOURCE OSMAttribute

#include "OSMAttribute.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMAttribute::g_Properties[] = 
{
{ "key", PIDX_key, offsetof(OSMAttribute, m_key), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "value", PIDX_value, offsetof(OSMAttribute, m_value), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool OSMAttribute::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMAttribute
