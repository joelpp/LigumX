#include "OSMElementComponent.h"

#pragma region  CLASS_SOURCE OSMElementComponent

#include "OSMElementComponent.h"
#include "serializer.h"
#include "Way.h"
const ClassPropertyData OSMElementComponent::g_Properties[] = 
{
{ "Way", PIDX_Way, offsetof(OSMElementComponent, m_Way), 0, LXType_ObjectPtr, sizeof(Way*), LXType_Way, true, LXType_None, false, 0, 0, 0, 0,}, 
};
void OSMElementComponent::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Way], m_Way);
}
bool OSMElementComponent::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementComponent::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Way], &m_Way  );
	return true;
}
const char* OSMElementComponent::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMElementComponent


void OSMElementComponent::Update()
{

}
