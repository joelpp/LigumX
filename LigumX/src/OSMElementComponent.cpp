#include "OSMElementComponent.h"

#pragma region  CLASS_SOURCE OSMElementComponent

#include "OSMElementComponent.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
#include "Way.h"
const ClassPropertyData OSMElementComponent::g_Properties[] = 
{
{ "Way", PIDX_Way, offsetof(OSMElementComponent, m_Way), 0, LXType_ObjectPtr, sizeof(Way*), LXType_Way, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool OSMElementComponent::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementComponent::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_OBJECTREF("Way", m_Way, Way);
	return true;
}

#pragma endregion  CLASS_SOURCE OSMElementComponent


void OSMElementComponent::Update()
{

}
