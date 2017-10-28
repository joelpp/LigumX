#include "Component.h"

#pragma region  CLASS_SOURCE Component

#include "Component.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "ObjectID", offsetof(Component, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(Component, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "ParentEntity", offsetof(Component, m_ParentEntity), 0, LXType_Entity, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
void Component::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE Component

