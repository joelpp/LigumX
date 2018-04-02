#pragma once

#include "glm/glm.hpp"
#include "LXError.h"

#pragma region  FORWARD_DECLARATIONS EditorTool
#include "property.h"

class EditorTool;


#pragma endregion  FORWARD_DECLARATIONS EditorTool

class EditorTool
{

#pragma region  HEADER EditorTool
public:
static const int ClassID = 4268435202;
static const LXType Type = LXType_EditorTool;
static constexpr const char* ClassName = "EditorTool";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetActive() { return m_Active; }; 
void SetActive(bool value) { m_Active = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Active = false;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Active,
};
bool Serialize(bool writing);

#pragma endregion  HEADER EditorTool

virtual bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	lxAssert0();
	return false;
}

virtual void HandleKeyboardInput(int button, int action, int mods)
{
}

};
