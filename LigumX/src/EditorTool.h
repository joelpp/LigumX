#pragma once

#include "glm/glm.hpp"
#include "LXError.h"

#pragma region  FORWARD_DECLARATIONS EditorTool
#include "LXSystem.h"
class Serializer2;

class EditorTool;


#pragma endregion  FORWARD_DECLARATIONS EditorTool

#pragma region  HEADER EditorTool
class EditorTool : public LXObject
{
public:
static const int ClassID = 779603202;
static const LXType Type = LXType_EditorTool;
static constexpr const char* ClassName = "EditorTool";
virtual LXType GetLXType() { return LXType_EditorTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetActive() { return m_Active; }; 
void SetActive(bool value) { m_Active = value; }; 
bool GetVisible() { return m_Visible; }; 
void SetVisible(bool value) { m_Visible = value; }; 
private:
bool m_Active = false;
bool m_Visible = false;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorToolPIDX
{
PIDX_Active,
PIDX_Visible,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(EditorTool& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER EditorTool

virtual void Reset() { };

virtual bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	return false;
}

virtual void DrawImguiWindow();

virtual void HandleKeyboardInput(int button, int action, int mods) { }
virtual bool HandleMouseScroll(const glm::vec2& scrolling) { return false; }
virtual void DebugDisplay() { }

};
