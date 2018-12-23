#include "EditorOptions.h"

#pragma region  CLASS_SOURCE EditorOptions

#include "EditorOptions.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EditorOptions::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(EditorOptions, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowWorldWindow", PIDX_ShowWorldWindow, offsetof(EditorOptions, m_ShowWorldWindow), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowMaterialWindow", PIDX_ShowMaterialWindow, offsetof(EditorOptions, m_ShowMaterialWindow), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowEntityWindow", PIDX_ShowEntityWindow, offsetof(EditorOptions, m_ShowEntityWindow), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowObjectCreator", PIDX_ShowObjectCreator, offsetof(EditorOptions, m_ShowObjectCreator), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowEngineStats", PIDX_ShowEngineStats, offsetof(EditorOptions, m_ShowEngineStats), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowTestGUI", PIDX_ShowTestGUI, offsetof(EditorOptions, m_ShowTestGUI), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowObjectManager", PIDX_ShowObjectManager, offsetof(EditorOptions, m_ShowObjectManager), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "BackupDataOnSave", PIDX_BackupDataOnSave, offsetof(EditorOptions, m_BackupDataOnSave), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugDisplay", PIDX_DebugDisplay, offsetof(EditorOptions, m_DebugDisplay), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayMessages", PIDX_DisplayMessages, offsetof(EditorOptions, m_DisplayMessages), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SaveDisabled", PIDX_SaveDisabled, offsetof(EditorOptions, m_SaveDisabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayPickingTool", PIDX_DisplayPickingTool, offsetof(EditorOptions, m_DisplayPickingTool), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayOSMTool", PIDX_DisplayOSMTool, offsetof(EditorOptions, m_DisplayOSMTool), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplaySectorTool", PIDX_DisplaySectorTool, offsetof(EditorOptions, m_DisplaySectorTool), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayFramebufferTool", PIDX_DisplayFramebufferTool, offsetof(EditorOptions, m_DisplayFramebufferTool), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayTerrainTool", PIDX_DisplayTerrainTool, offsetof(EditorOptions, m_DisplayTerrainTool), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayAxisGizmo", PIDX_DisplayAxisGizmo, offsetof(EditorOptions, m_DisplayAxisGizmo), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MouseScrollCameraSpeed", PIDX_MouseScrollCameraSpeed, offsetof(EditorOptions, m_MouseScrollCameraSpeed), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MouseScrollEntityCloseupPercent", PIDX_MouseScrollEntityCloseupPercent, offsetof(EditorOptions, m_MouseScrollEntityCloseupPercent), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
bool EditorOptions::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EditorOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("Enabled", m_Enabled);
	LXIMGUI_SHOW_BOOL("ShowWorldWindow", m_ShowWorldWindow);
	LXIMGUI_SHOW_BOOL("ShowMaterialWindow", m_ShowMaterialWindow);
	LXIMGUI_SHOW_BOOL("ShowEntityWindow", m_ShowEntityWindow);
	LXIMGUI_SHOW_BOOL("ShowObjectCreator", m_ShowObjectCreator);
	LXIMGUI_SHOW_BOOL("ShowEngineStats", m_ShowEngineStats);
	LXIMGUI_SHOW_BOOL("ShowTestGUI", m_ShowTestGUI);
	LXIMGUI_SHOW_BOOL("ShowObjectManager", m_ShowObjectManager);
	LXIMGUI_SHOW_BOOL("BackupDataOnSave", m_BackupDataOnSave);
	LXIMGUI_SHOW_BOOL("DebugDisplay", m_DebugDisplay);
	LXIMGUI_SHOW_BOOL("DisplayMessages", m_DisplayMessages);
	LXIMGUI_SHOW_BOOL("SaveDisabled", m_SaveDisabled);
	LXIMGUI_SHOW_BOOL("DisplayPickingTool", m_DisplayPickingTool);
	LXIMGUI_SHOW_BOOL("DisplayOSMTool", m_DisplayOSMTool);
	LXIMGUI_SHOW_BOOL("DisplaySectorTool", m_DisplaySectorTool);
	LXIMGUI_SHOW_BOOL("DisplayFramebufferTool", m_DisplayFramebufferTool);
	LXIMGUI_SHOW_BOOL("DisplayTerrainTool", m_DisplayTerrainTool);
	LXIMGUI_SHOW_BOOL("DisplayAxisGizmo", m_DisplayAxisGizmo);
	LXIMGUI_SHOW_FLOAT("MouseScrollCameraSpeed", m_MouseScrollCameraSpeed, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("MouseScrollEntityCloseupPercent", m_MouseScrollEntityCloseupPercent, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	return true;
}

#pragma endregion  CLASS_SOURCE EditorOptions

EditorOptions::EditorOptions()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}
