#include "Component.h"

#pragma region  CLASS_SOURCE Component

#include "Component.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "ParentEntity", PIDX_ParentEntity, offsetof(Component, m_ParentEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
bool Component::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
void Component::ShowPropertyGrid()
{
}

#pragma endregion  CLASS_SOURCE Component

