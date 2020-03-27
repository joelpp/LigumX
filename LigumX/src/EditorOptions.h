#pragma once

#pragma region  FORWARD_DECLARATIONS EditorOptions
#include "LXSystem.h"
class Serializer2;

class EditorOptions;


#pragma endregion  FORWARD_DECLARATIONS EditorOptions

#pragma region  HEADER EditorOptions
class EditorOptions : public LXObject
{
public:
static const int ClassID = -1054205242;
static const LXType Type = LXType_EditorOptions;
static constexpr const char* ClassName = "EditorOptions";
typedef LXObject super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
bool GetShowWorldWindow() { return m_ShowWorldWindow; }; 
void SetShowWorldWindow(bool value) { m_ShowWorldWindow = value; }; 
bool GetShowMaterialWindow() { return m_ShowMaterialWindow; }; 
void SetShowMaterialWindow(bool value) { m_ShowMaterialWindow = value; }; 
bool GetShowEntityWindow() { return m_ShowEntityWindow; }; 
void SetShowEntityWindow(bool value) { m_ShowEntityWindow = value; }; 
bool GetShowObjectCreator() { return m_ShowObjectCreator; }; 
void SetShowObjectCreator(bool value) { m_ShowObjectCreator = value; }; 
bool GetShowEngineStats() { return m_ShowEngineStats; }; 
void SetShowEngineStats(bool value) { m_ShowEngineStats = value; }; 
bool GetShowTestGUI() { return m_ShowTestGUI; }; 
void SetShowTestGUI(bool value) { m_ShowTestGUI = value; }; 
bool GetShowObjectManager() { return m_ShowObjectManager; }; 
void SetShowObjectManager(bool value) { m_ShowObjectManager = value; }; 
bool GetBackupDataOnSave() { return m_BackupDataOnSave; }; 
void SetBackupDataOnSave(bool value) { m_BackupDataOnSave = value; }; 
bool GetDebugDisplay() { return m_DebugDisplay; }; 
void SetDebugDisplay(bool value) { m_DebugDisplay = value; }; 
bool GetDisplayMessages() { return m_DisplayMessages; }; 
void SetDisplayMessages(bool value) { m_DisplayMessages = value; }; 
bool GetSaveDisabled() { return m_SaveDisabled; }; 
void SetSaveDisabled(bool value) { m_SaveDisabled = value; }; 
bool GetDisplayPickingTool() { return m_DisplayPickingTool; }; 
void SetDisplayPickingTool(bool value) { m_DisplayPickingTool = value; }; 
bool GetDisplayOSMTool() { return m_DisplayOSMTool; }; 
void SetDisplayOSMTool(bool value) { m_DisplayOSMTool = value; }; 
bool GetDisplaySectorTool() { return m_DisplaySectorTool; }; 
void SetDisplaySectorTool(bool value) { m_DisplaySectorTool = value; }; 
bool GetDisplayFramebufferTool() { return m_DisplayFramebufferTool; }; 
void SetDisplayFramebufferTool(bool value) { m_DisplayFramebufferTool = value; }; 
bool GetDisplayTerrainTool() { return m_DisplayTerrainTool; }; 
void SetDisplayTerrainTool(bool value) { m_DisplayTerrainTool = value; }; 
bool GetDisplayAxisGizmo() { return m_DisplayAxisGizmo; }; 
void SetDisplayAxisGizmo(bool value) { m_DisplayAxisGizmo = value; }; 
float GetMouseScrollCameraSpeed() { return m_MouseScrollCameraSpeed; }; 
void SetMouseScrollCameraSpeed(float value) { m_MouseScrollCameraSpeed = value; }; 
float GetMouseScrollEntityCloseupPercent() { return m_MouseScrollEntityCloseupPercent; }; 
void SetMouseScrollEntityCloseupPercent(float value) { m_MouseScrollEntityCloseupPercent = value; }; 
private:
bool m_Enabled = false;
bool m_ShowWorldWindow = false;
bool m_ShowMaterialWindow = false;
bool m_ShowEntityWindow = false;
bool m_ShowObjectCreator = false;
bool m_ShowEngineStats = false;
bool m_ShowTestGUI = false;
bool m_ShowObjectManager = false;
bool m_BackupDataOnSave = false;
bool m_DebugDisplay = false;
bool m_DisplayMessages = true;
bool m_SaveDisabled = false;
bool m_DisplayPickingTool = false;
bool m_DisplayOSMTool = false;
bool m_DisplaySectorTool = true;
bool m_DisplayFramebufferTool = true;
bool m_DisplayTerrainTool = true;
bool m_DisplayAxisGizmo = true;
float m_MouseScrollCameraSpeed = 10.0f;
float m_MouseScrollEntityCloseupPercent = 15.0f;
public:
static const int g_PropertyCount = 20;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorOptionsPIDX
{
PIDX_Enabled,
PIDX_ShowWorldWindow,
PIDX_ShowMaterialWindow,
PIDX_ShowEntityWindow,
PIDX_ShowObjectCreator,
PIDX_ShowEngineStats,
PIDX_ShowTestGUI,
PIDX_ShowObjectManager,
PIDX_BackupDataOnSave,
PIDX_DebugDisplay,
PIDX_DisplayMessages,
PIDX_SaveDisabled,
PIDX_DisplayPickingTool,
PIDX_DisplayOSMTool,
PIDX_DisplaySectorTool,
PIDX_DisplayFramebufferTool,
PIDX_DisplayTerrainTool,
PIDX_DisplayAxisGizmo,
PIDX_MouseScrollCameraSpeed,
PIDX_MouseScrollEntityCloseupPercent,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER EditorOptions

EditorOptions();

};
