#pragma once
#include "imgui_impl_glfw_gl3.h"
#include "GLFW\glfw3.h"
#include <vector>

#pragma region  FORWARD_DECLARATIONS GUI
#include "property.h"

class GUI;

extern GUI* g_GUI;

#pragma endregion  FORWARD_DECLARATIONS GUI

class GUI
{
#pragma region  HEADER GUI
public:
static const int ClassID = 2047816512;
static const LXType Type = LXType_GUI;
static constexpr const char* ClassName = "GUI";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& Getm_Enabled() { return m_m_Enabled; }; 
void Setm_Enabled(bool value) { m_m_Enabled = value; }; 
const int& GetNumActiveWindows() { return m_NumActiveWindows; }; 
void SetNumActiveWindows(int value) { m_NumActiveWindows = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_m_Enabled = false;
int m_NumActiveWindows = 0;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_GUIPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_m_Enabled,
PIDX_NumActiveWindows,
};
bool Serialize(bool writing);

#pragma endregion  HEADER GUI

GUI();

void BeginWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name);

void EndWindow();

void BeginFrame();
void EndFrame();

bool ProcessMouseScroll(float xOffset, float yOffset);

GLFWwindow* m_WindowHWPtr;
};
