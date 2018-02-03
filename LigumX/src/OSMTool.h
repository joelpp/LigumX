#pragma once
#include <vector>
#include "glm/glm.hpp"

#pragma region  FORWARD_DECLARATIONS OSMTool
#include "property.h"

class OSMTool;
class Node;
class Way;


#pragma endregion  FORWARD_DECLARATIONS OSMTool

class OSMTool
{
#pragma region  HEADER OSMTool
public:
static const int ClassID = 878880296;
static const LXType Type = LXType_OSMTool;
static constexpr const char* ClassName = "OSMTool";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
Node*& GetSelectedNode() { return m_SelectedNode; }; 
void SetSelectedNode(Node* value) { m_SelectedNode = value; }; 
std::vector<Way*>& GetSelectedWays() { return m_SelectedWays; }; 
void SetSelectedWays(std::vector<Way*> value) { m_SelectedWays = value; }; 
void AddTo_SelectedWays(Way* value) { m_SelectedWays.push_back(value); };
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = false;
Node* m_SelectedNode;
std::vector<Way*> m_SelectedWays;
public:
static const int g_PropertyCount = 5;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_SelectedNode,
PIDX_SelectedWays,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMTool

bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);


};
