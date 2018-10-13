#include "OSMElementComponent.h"

#pragma region  CLASS_SOURCE OSMElementComponent

#include "OSMElementComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMElementComponent::g_Properties[] = 
{
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
