#pragma once
#include "glm\glm.hpp"
#include "GL.h"

#pragma region  FORWARD_DECLARATIONS MainWindow
#include "LXSystem.h"
class Serializer2;

class MainWindow;


#pragma endregion  FORWARD_DECLARATIONS MainWindow

#pragma region  HEADER MainWindow
class MainWindow : public LXObject
{
public:
static const int ClassID = 2995597492;
static const LXType Type = LXType_MainWindow;
static constexpr const char* ClassName = "MainWindow";
typedef LXObject super;

bool GetInFocus() { return m_InFocus; }; 
void SetInFocus(bool value) { m_InFocus = value; }; 
glm::vec2& GetPosition() { return m_Position; }; 
void SetPosition(const glm::vec2& value) { m_Position = value; }; 
glm::ivec2& GetSize() { return m_Size; }; 
void SetSize(const glm::ivec2& value) { m_Size = value; }; 
const std::string& GetTitle() { return m_Title; }; 
void SetTitle(std::string value) { m_Title = value; }; 
private:
bool m_InFocus = false;
glm::vec2 m_Position = glm::vec2(0, 0);
glm::ivec2 m_Size = glm::ivec2(0, 0);
std::string m_Title;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MainWindowPIDX
{
PIDX_InFocus,
PIDX_Position,
PIDX_Size,
PIDX_Title,
};
bool Serialize(Serializer2& serializer);
bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER MainWindow

MainWindow();
MainWindow(int objectID);


GLFWwindow* GetHWObject() { return pWindow; }

GLFWwindow* pWindow;


};
