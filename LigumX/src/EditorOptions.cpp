#include "EditorOptions.h"

#pragma region  CLASS_SOURCE EditorOptions

#include "EditorOptions.h"
#include "serializer.h"
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
void EditorOptions::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
	serializer.SerializeBool(g_Properties[PIDX_ShowWorldWindow], m_ShowWorldWindow);
	serializer.SerializeBool(g_Properties[PIDX_ShowMaterialWindow], m_ShowMaterialWindow);
	serializer.SerializeBool(g_Properties[PIDX_ShowEntityWindow], m_ShowEntityWindow);
	serializer.SerializeBool(g_Properties[PIDX_ShowObjectCreator], m_ShowObjectCreator);
	serializer.SerializeBool(g_Properties[PIDX_ShowEngineStats], m_ShowEngineStats);
	serializer.SerializeBool(g_Properties[PIDX_ShowTestGUI], m_ShowTestGUI);
	serializer.SerializeBool(g_Properties[PIDX_ShowObjectManager], m_ShowObjectManager);
	serializer.SerializeBool(g_Properties[PIDX_BackupDataOnSave], m_BackupDataOnSave);
	serializer.SerializeBool(g_Properties[PIDX_DebugDisplay], m_DebugDisplay);
	serializer.SerializeBool(g_Properties[PIDX_DisplayMessages], m_DisplayMessages);
	serializer.SerializeBool(g_Properties[PIDX_SaveDisabled], m_SaveDisabled);
	serializer.SerializeBool(g_Properties[PIDX_DisplayPickingTool], m_DisplayPickingTool);
	serializer.SerializeBool(g_Properties[PIDX_DisplayOSMTool], m_DisplayOSMTool);
	serializer.SerializeBool(g_Properties[PIDX_DisplaySectorTool], m_DisplaySectorTool);
	serializer.SerializeBool(g_Properties[PIDX_DisplayFramebufferTool], m_DisplayFramebufferTool);
	serializer.SerializeBool(g_Properties[PIDX_DisplayTerrainTool], m_DisplayTerrainTool);
	serializer.SerializeBool(g_Properties[PIDX_DisplayAxisGizmo], m_DisplayAxisGizmo);
	serializer.SerializeFloat(g_Properties[PIDX_MouseScrollCameraSpeed], m_MouseScrollCameraSpeed);
	serializer.SerializeFloat(g_Properties[PIDX_MouseScrollEntityCloseupPercent], m_MouseScrollEntityCloseupPercent);
}
bool EditorOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EditorOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowWorldWindow], &m_ShowWorldWindow  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowMaterialWindow], &m_ShowMaterialWindow  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowEntityWindow], &m_ShowEntityWindow  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowObjectCreator], &m_ShowObjectCreator  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowEngineStats], &m_ShowEngineStats  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowTestGUI], &m_ShowTestGUI  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowObjectManager], &m_ShowObjectManager  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_BackupDataOnSave], &m_BackupDataOnSave  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DebugDisplay], &m_DebugDisplay  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayMessages], &m_DisplayMessages  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SaveDisabled], &m_SaveDisabled  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayPickingTool], &m_DisplayPickingTool  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayOSMTool], &m_DisplayOSMTool  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplaySectorTool], &m_DisplaySectorTool  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayFramebufferTool], &m_DisplayFramebufferTool  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayTerrainTool], &m_DisplayTerrainTool  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayAxisGizmo], &m_DisplayAxisGizmo  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MouseScrollCameraSpeed], &m_MouseScrollCameraSpeed , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_MouseScrollEntityCloseupPercent], &m_MouseScrollEntityCloseupPercent , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
const char* EditorOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE EditorOptions

EditorOptions::EditorOptions()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}
