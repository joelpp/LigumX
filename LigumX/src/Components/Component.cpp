#include "Component.h"

#pragma region  CLASS_SOURCE Component

#include "Component.h"
#include "serializer.h"
#include "Entity.h"
const ClassPropertyData Component::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(Component, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ParentEntity", PIDX_ParentEntity, offsetof(Component, m_ParentEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
void Component::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
}
bool Component::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Component::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_ParentEntity], &m_ParentEntity  );
	return true;
}
void Component::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Component* other = (Component*) otherObj;
	other->SetEnabled(m_Enabled);
	other->SetParentEntity(m_ParentEntity);
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
