#include "Component.h"

#pragma region  CLASS_SOURCE Component

#include "Component.h"
#include "serializer.h"
#include "Entity.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "ParentEntity", PIDX_ParentEntity, offsetof(Component, m_ParentEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
bool Component::Serialize(Serializer2& serializer)
{
	return true;
}
bool Component::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Component::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_OBJECTREF("ParentEntity", m_ParentEntity);
	return true;
}
const char* Component::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Component

