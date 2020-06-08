#include "EditorTool.h"

#pragma region  CLASS_SOURCE EditorTool

#include "EditorTool.h"
#include "serializer.h"
const ClassPropertyData EditorTool::g_Properties[] = 
{
{ "Active", PIDX_Active, offsetof(EditorTool, m_Active), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Visible", PIDX_Visible, offsetof(EditorTool, m_Visible), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void EditorTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Active], m_Active);
	serializer.SerializeBool(g_Properties[PIDX_Visible], m_Visible);
}
bool EditorTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EditorTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Active], &m_Active  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Visible], &m_Visible  );
	return true;
}
void EditorTool::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	EditorTool* other = (EditorTool*) otherObj;
	other->SetActive(m_Active);
	other->SetVisible(m_Visible);
}
const char* EditorTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE EditorTool

void EditorTool::DrawImguiWindow()
{

}
