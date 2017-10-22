#include "Component.h"

#pragma region  CLASS_SOURCE Component
#include "Component.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "ObjectID", offsetof(Component, m_ObjectID), 0, LXType_int, false, LXType_None, PropertyFlags_NonEditable, 0, 0, }, 
{ "ParentEntity", offsetof(Component, m_ParentEntity), 0, LXType_Entity, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
void Component::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE Component

