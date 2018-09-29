#pragma once
#include "glm\glm.hpp"
#include "GL.h"

#pragma region  FORWARD_DECLARATIONS MainWindow
#include "property.h"

class MainWindow;


#pragma endregion  FORWARD_DECLARATIONS MainWindow

class MainWindow
{
#pragma region  HEADER MainWindow
public:
static const int ClassID = 2995597492;
static const LXType Type = LXType_MainWindow;
static constexpr const char* ClassName = "MainWindow";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetInFocus() { return m_InFocus; }; 
void SetInFocus(bool value) { m_InFocus = value; }; 
glm::vec2& GetPosition() { return m_Position; }; 
void SetPosition(const glm::vec2& value) { m_Position = value; }; 
glm::ivec2& GetSize() { return m_Size; }; 
void SetSize(const glm::ivec2& value) { m_Size = value; }; 
const std::string& GetTitle() { return m_Title; }; 
void SetTitle(std::string value) { m_Title = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_InFocus = false;
glm::vec2 m_Position = glm::vec2(0, 0);
glm::ivec2 m_Size = glm::ivec2(0, 0);
std::string m_Title;
public:
static const int g_PropertyCount = 6;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MainWindowPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_InFocus,
PIDX_Position,
PIDX_Size,
PIDX_Title,
};
bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);

#pragma endregion  HEADER MainWindow

MainWindow();
MainWindow(int objectID);


GLFWwindow* GetHWObject() { return pWindow; }

GLFWwindow* pWindow;


};
