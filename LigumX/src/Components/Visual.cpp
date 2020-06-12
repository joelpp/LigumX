#include "Visual.h"

#pragma region  CLASS_SOURCE Visual

#include "Visual.h"
#include "serializer.h"
#include "Model.h"
const ClassPropertyData Visual::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(Visual, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Model", PIDX_Model, offsetof(Visual, m_Model), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, 0, 0, 0, 0,}, 
};
void Visual::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Model], m_Model);
}
bool Visual::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Visual::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Model], &m_Model  );
	return true;
}
void Visual::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Visual* other = (Visual*) otherObj;
	other->SetEnabled(m_Enabled);
	other->SetModel(m_Model);
}
const char* Visual::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Visual