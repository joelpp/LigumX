#pragma once
#include "imgui_impl_glfw_gl3.h"

#pragma region  FORWARD_DECLARATIONS GUI
#include "LXSystem.h"
class Serializer2;

class GUI;

extern GUI* g_GUI;

#pragma endregion  FORWARD_DECLARATIONS GUI

#pragma region  HEADER GUI
class GUI : public LXObject
{
public:
static const int ClassID = -1741320384;
static const LXType Type = LXType_GUI;
static constexpr const char* ClassName = "GUI";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool Getm_Enabled() { return m_m_Enabled; }; 
void Setm_Enabled(bool value) { m_m_Enabled = value; }; 
int GetNumActiveWindows() { return m_NumActiveWindows; }; 
void SetNumActiveWindows(int value) { m_NumActiveWindows = value; }; 
private:
bool m_m_Enabled = false;
int m_NumActiveWindows = 0;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GUIPIDX
{
PIDX_m_Enabled,
PIDX_NumActiveWindows,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(GUI& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER GUI

GUI();

void BeginWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name);

void EndWindow();

void BeginFrame();
void EndFrame();

bool ProcessMouseScroll(float xOffset, float yOffset);

GLFWwindow* m_WindowHWPtr;
};
