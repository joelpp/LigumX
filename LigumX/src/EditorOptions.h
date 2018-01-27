#pragma once

#pragma region  FORWARD_DECLARATIONS EditorOptions
#include "property.h"

class EditorOptions;


#pragma endregion  FORWARD_DECLARATIONS EditorOptions

class EditorOptions
{
#pragma region  HEADER EditorOptions
public:
static const int ClassID = 3240762054;
static const LXType Type = LXType_EditorOptions;
static constexpr const char* ClassName = "EditorOptions";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
const bool& GetShowWorldWindow() { return m_ShowWorldWindow; }; 
void SetShowWorldWindow(bool value) { m_ShowWorldWindow = value; }; 
const bool& GetShowMaterialWindow() { return m_ShowMaterialWindow; }; 
void SetShowMaterialWindow(bool value) { m_ShowMaterialWindow = value; }; 
const bool& GetShowEntityWindow() { return m_ShowEntityWindow; }; 
void SetShowEntityWindow(bool value) { m_ShowEntityWindow = value; }; 
const bool& GetShowObjectCreator() { return m_ShowObjectCreator; }; 
void SetShowObjectCreator(bool value) { m_ShowObjectCreator = value; }; 
const bool& GetShowEngineStats() { return m_ShowEngineStats; }; 
void SetShowEngineStats(bool value) { m_ShowEngineStats = value; }; 
const bool& GetShowTestGUI() { return m_ShowTestGUI; }; 
void SetShowTestGUI(bool value) { m_ShowTestGUI = value; }; 
const bool& GetShowObjectManager() { return m_ShowObjectManager; }; 
void SetShowObjectManager(bool value) { m_ShowObjectManager = value; }; 
const bool& GetBackupDataOnSave() { return m_BackupDataOnSave; }; 
void SetBackupDataOnSave(bool value) { m_BackupDataOnSave = value; }; 
const bool& GetDebugDisplay() { return m_DebugDisplay; }; 
void SetDebugDisplay(bool value) { m_DebugDisplay = value; }; 
const bool& GetSaveDisabled() { return m_SaveDisabled; }; 
void SetSaveDisabled(bool value) { m_SaveDisabled = value; }; 
const float& GetMouseScrollCameraSpeed() { return m_MouseScrollCameraSpeed; }; 
void SetMouseScrollCameraSpeed(float value) { m_MouseScrollCameraSpeed = value; }; 
private:
int m_ObjectID
;
std::string m_Name
;
bool m_Enabled
 = false;
bool m_ShowWorldWindow
 = false;
bool m_ShowMaterialWindow
 = false;
bool m_ShowEntityWindow
 = false;
bool m_ShowObjectCreator
 = false;
bool m_ShowEngineStats
 = false;
bool m_ShowTestGUI
 = false;
bool m_ShowObjectManager
 = false;
bool m_BackupDataOnSave
 = false;
bool m_DebugDisplay
 = false;
bool m_SaveDisabled
 = false;
float m_MouseScrollCameraSpeed
 = 1.0f;
public:
static const int g_PropertyCount = 14;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorOptionsPIDX
{
PIDX_ObjectID,
PIDX_Name,
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
PIDX_SaveDisabled,
PIDX_MouseScrollCameraSpeed,
};
bool Serialize(bool writing);

#pragma endregion  HEADER EditorOptions

EditorOptions();

};
