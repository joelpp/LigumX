#include "OSMAttribute.h"

#pragma region  CLASS_SOURCE OSMAttribute

#include "OSMAttribute.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMAttribute::g_Properties[] = 
{
{ "Key", PIDX_Key, offsetof(OSMAttribute, m_Key), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Value", PIDX_Value, offsetof(OSMAttribute, m_Value), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool OSMAttribute::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMAttribute


OSMAttribute::OSMAttribute(const std::string& _key, const std::string& _value)
	: m_Key(_key)
	, m_Value(_value)
{

}
