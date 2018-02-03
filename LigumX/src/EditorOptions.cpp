#include "EditorOptions.h"

#pragma region  CLASS_SOURCE EditorOptions

#include "EditorOptions.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EditorOptions::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(EditorOptions, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(EditorOptions, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", PIDX_Enabled, offsetof(EditorOptions, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowWorldWindow", PIDX_ShowWorldWindow, offsetof(EditorOptions, m_ShowWorldWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowMaterialWindow", PIDX_ShowMaterialWindow, offsetof(EditorOptions, m_ShowMaterialWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowEntityWindow", PIDX_ShowEntityWindow, offsetof(EditorOptions, m_ShowEntityWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowObjectCreator", PIDX_ShowObjectCreator, offsetof(EditorOptions, m_ShowObjectCreator), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowEngineStats", PIDX_ShowEngineStats, offsetof(EditorOptions, m_ShowEngineStats), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowTestGUI", PIDX_ShowTestGUI, offsetof(EditorOptions, m_ShowTestGUI), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowObjectManager", PIDX_ShowObjectManager, offsetof(EditorOptions, m_ShowObjectManager), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "BackupDataOnSave", PIDX_BackupDataOnSave, offsetof(EditorOptions, m_BackupDataOnSave), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DebugDisplay", PIDX_DebugDisplay, offsetof(EditorOptions, m_DebugDisplay), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SaveDisabled", PIDX_SaveDisabled, offsetof(EditorOptions, m_SaveDisabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DisplayPickingTool", PIDX_DisplayPickingTool, offsetof(EditorOptions, m_DisplayPickingTool), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DisplayOSMTool", PIDX_DisplayOSMTool, offsetof(EditorOptions, m_DisplayOSMTool), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DisplaySectorTool", PIDX_DisplaySectorTool, offsetof(EditorOptions, m_DisplaySectorTool), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "MouseScrollCameraSpeed", PIDX_MouseScrollCameraSpeed, offsetof(EditorOptions, m_MouseScrollCameraSpeed), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "MouseScrollEntityCloseupPercent", PIDX_MouseScrollEntityCloseupPercent, offsetof(EditorOptions, m_MouseScrollEntityCloseupPercent), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
};
bool EditorOptions::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE EditorOptions

EditorOptions::EditorOptions()
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}
