#pragma once

#pragma region  FORWARD_DECLARATIONS ObjectTool
#include "LXSystem.h"
class Serializer2;

class ObjectTool;


#pragma endregion  FORWARD_DECLARATIONS ObjectTool

#include "EditorTool.h"

#pragma region  HEADER ObjectTool
class ObjectTool : public EditorTool
{
public:
static const int ClassID = 987879604;
static const LXType Type = LXType_ObjectTool;
static constexpr const char* ClassName = "ObjectTool";
virtual LXType GetLXType() { return LXType_ObjectTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

int GetSelectedFileIndex() { return m_SelectedFileIndex; }; 
void SetSelectedFileIndex(int value) { m_SelectedFileIndex = value; }; 
bool GetLoadUnloadedObjects() { return m_LoadUnloadedObjects; }; 
void SetLoadUnloadedObjects(bool value) { m_LoadUnloadedObjects = value; }; 
bool GetCOMMAND_SaveCurrentObject() { return m_COMMAND_SaveCurrentObject; }; 
void SetCOMMAND_SaveCurrentObject(bool value) { SetCOMMAND_SaveCurrentObject_Callback(value); }; 
void SetCOMMAND_SaveCurrentObject_Callback(const bool& value);
bool GetCOMMAND_CreateNewObject() { return m_COMMAND_CreateNewObject; }; 
void SetCOMMAND_CreateNewObject(bool value) { SetCOMMAND_CreateNewObject_Callback(value); }; 
void SetCOMMAND_CreateNewObject_Callback(const bool& value);
bool GetCOMMAND_CloneCurrentObject() { return m_COMMAND_CloneCurrentObject; }; 
void SetCOMMAND_CloneCurrentObject(bool value) { SetCOMMAND_CloneCurrentObject_Callback(value); }; 
void SetCOMMAND_CloneCurrentObject_Callback(const bool& value);
private:
int m_SelectedFileIndex = -1;
bool m_LoadUnloadedObjects = false;
bool m_COMMAND_SaveCurrentObject = false;
bool m_COMMAND_CreateNewObject = false;
bool m_COMMAND_CloneCurrentObject = false;
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ObjectToolPIDX
{
PIDX_SelectedFileIndex,
PIDX_LoadUnloadedObjects,
PIDX_COMMAND_SaveCurrentObject,
PIDX_COMMAND_CreateNewObject,
PIDX_COMMAND_CloneCurrentObject,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER ObjectTool

virtual bool Process(bool isActiveTool, bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);

virtual void DrawImguiWindow();

LXObject* m_CurrentObject = nullptr;

void HandleFileDrop(LXType droppedType, StringList& tokens);

private:

	void CreateNewObject(const LXString& typeName);

};
