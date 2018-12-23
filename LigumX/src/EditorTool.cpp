#include "EditorTool.h"

#pragma region  CLASS_SOURCE EditorTool

#include "EditorTool.h"
const ClassPropertyData EditorTool::g_Properties[] = 
{
{ "Active", PIDX_Active, offsetof(EditorTool, m_Active), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool EditorTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EditorTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("Active", m_Active);
	return true;
}

#pragma endregion  CLASS_SOURCE EditorTool
