#pragma once
#include "glm\glm.hpp"

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
const glm::vec2& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec2 value) { m_Position = value; }; 
const glm::vec2& GetSize() { return m_Size; }; 
void SetSize(glm::vec2 value) { m_Size = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_InFocus = false;
glm::vec2 m_Position = glm::vec2(0, 0);
glm::vec2 m_Size = glm::vec2(0, 0);
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MainWindowPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_InFocus,
PIDX_Position,
PIDX_Size,
};
bool Serialize(bool writing);

#pragma endregion  HEADER MainWindow

};
