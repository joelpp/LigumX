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
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

int GetSelectedFileIndex() { return m_SelectedFileIndex; }; 
void SetSelectedFileIndex(int value) { m_SelectedFileIndex = value; }; 
private:
int m_SelectedFileIndex = 0;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ObjectToolPIDX
{
PIDX_SelectedFileIndex,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER ObjectTool

virtual bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);

virtual void DrawImguiWindow();

LXObject* m_CurrentObject = nullptr;
};
