#include "Component.h"

#pragma region  CLASS_SOURCE Component

#include "Component.h"
#include "serializer.h"
#include "Entity.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "ParentEntity", PIDX_ParentEntity, offsetof(Component, m_ParentEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
void Component::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.Close();
}
bool Component::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Component::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObjectPtr("ParentEntity", m_ParentEntity  );
	return true;
}
const char* Component::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Component

void Component::Update()
{
	static bool g_Debug_ComponentUpdateControl = false;
	if (g_Debug_ComponentUpdateControl)
	{
		lxAssert0();
	}
}
