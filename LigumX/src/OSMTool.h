#pragma once
#include <vector>
#include "glm/glm.hpp"

#include "EditorTool.h"

class Sector;

#pragma region  FORWARD_DECLARATIONS OSMTool
#include "property.h"

class OSMTool;
class Node;
class Way;


#pragma endregion  FORWARD_DECLARATIONS OSMTool

class OSMTool : public EditorTool
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
const bool& GetSearchOnlyWithinSector() { return m_SearchOnlyWithinSector; }; 
void SetSearchOnlyWithinSector(bool value) { m_SearchOnlyWithinSector = value; }; 
glm::ivec2& GetSelectedSectorIndex() { return m_SelectedSectorIndex; }; 
void SetSelectedSectorIndex(const glm::ivec2& value) { m_SelectedSectorIndex = value; }; 
glm::vec3& GetWorldSpacePosition() { return m_WorldSpacePosition; }; 
void SetWorldSpacePosition(const glm::vec3& value) { m_WorldSpacePosition = value; }; 
const bool& GetShowNodes() { return m_ShowNodes; }; 
void SetShowNodes(bool value) { m_ShowNodes = value; }; 
const bool& GetShowWays() { return m_ShowWays; }; 
void SetShowWays(bool value) { m_ShowWays = value; }; 
const bool& GetShowFlatWays() { return m_ShowFlatWays; }; 
void SetShowFlatWays(bool value) { m_ShowFlatWays = value; }; 
const bool& GetDebugPointInRoad() { return m_DebugPointInRoad; }; 
void SetDebugPointInRoad(bool value) { m_DebugPointInRoad = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = true;
Node* m_SelectedNode = nullptr;
std::vector<Way*> m_SelectedWays;
bool m_SearchOnlyWithinSector = false;
glm::ivec2 m_SelectedSectorIndex = glm::ivec2(0, 0);
glm::vec3 m_WorldSpacePosition = glm::vec3(0, 0, 0);
bool m_ShowNodes = false;
bool m_ShowWays = false;
bool m_ShowFlatWays = true;
bool m_DebugPointInRoad = false;
public:
static const int g_PropertyCount = 12;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_SelectedNode,
PIDX_SelectedWays,
PIDX_SearchOnlyWithinSector,
PIDX_SelectedSectorIndex,
PIDX_WorldSpacePosition,
PIDX_ShowNodes,
PIDX_ShowWays,
PIDX_ShowFlatWays,
PIDX_DebugPointInRoad,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMTool

OSMTool();

bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;

void DebugDisplay() override;
void DisplaySectorDebug(Sector* sector);

std::vector<glm::vec3>& GetWayDebugColors() { return m_WayDebugColors; };
std::vector<int>& GetWayDisplayToggles() { return m_WayDisplayToggles; };

private:

std::vector<int> m_WayDisplayToggles;
std::vector<glm::vec3> m_WayDebugColors;
};
