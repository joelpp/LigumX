#include "OSMElementComponent.h"

#pragma region  CLASS_SOURCE OSMElementComponent

#include "OSMElementComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMElementComponent::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(OSMElementComponent, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(OSMElementComponent, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "Way", PIDX_Way, offsetof(OSMElementComponent, m_Way), 0, LXType_Way, true, LXType_None, 0, 0, 0, 0,}, 
};
bool OSMElementComponent::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMElementComponent


void OSMElementComponent::Update()
{

}
