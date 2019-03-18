#include "EditorTool.h"

#pragma region  CLASS_SOURCE EditorTool

#include "EditorTool.h"
#include "serializer.h"
const ClassPropertyData EditorTool::g_Properties[] = 
{
{ "Active", PIDX_Active, offsetof(EditorTool, m_Active), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void EditorTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool("Active", m_Active);
}
bool EditorTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EditorTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("Active", m_Active  );
	return true;
}
const char* EditorTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE EditorTool
