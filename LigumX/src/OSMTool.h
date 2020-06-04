#pragma once
#include <vector>
#include "glm/glm.hpp"

#include "EditorTool.h"

class Sector;

#pragma region  FORWARD_DECLARATIONS OSMTool
#include "LXSystem.h"
class Serializer2;

class OSMTool;
class Node;
class Way;
class OSMElementComponent;


#pragma endregion  FORWARD_DECLARATIONS OSMTool

#pragma region  HEADER OSMTool
class OSMTool : public EditorTool
{
public:
static const int ClassID = 1416751752;
static const LXType Type = LXType_OSMTool;
static constexpr const char* ClassName = "OSMTool";
virtual LXType GetLXType() { return LXType_OSMTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
Node*& GetSelectedNode() { return m_SelectedNode; }; 
void SetSelectedNode(Node* value) { m_SelectedNode = value; }; 
std::vector<Way*>& GetSelectedWays() { return m_SelectedWays; }; 
void SetSelectedWays(std::vector<Way*> value) { m_SelectedWays = value; }; 
void AddTo_SelectedWays(Way* value) { m_SelectedWays.push_back(value); };
OSMElementComponent*& GetSelectedOSMElementComponent() { return m_SelectedOSMElementComponent; }; 
void SetSelectedOSMElementComponent(OSMElementComponent* value) { m_SelectedOSMElementComponent = value; }; 
bool GetSearchOnlyWithinSector() { return m_SearchOnlyWithinSector; }; 
void SetSearchOnlyWithinSector(bool value) { m_SearchOnlyWithinSector = value; }; 
glm::ivec2& GetSelectedSectorIndex() { return m_SelectedSectorIndex; }; 
void SetSelectedSectorIndex(const glm::ivec2& value) { m_SelectedSectorIndex = value; }; 
glm::vec3& GetWorldSpacePosition() { return m_WorldSpacePosition; }; 
void SetWorldSpacePosition(const glm::vec3& value) { m_WorldSpacePosition = value; }; 
bool GetShowNodes() { return m_ShowNodes; }; 
void SetShowNodes(bool value) { m_ShowNodes = value; }; 
bool GetShowWays() { return m_ShowWays; }; 
void SetShowWays(bool value) { m_ShowWays = value; }; 
bool GetShowFlatWays() { return m_ShowFlatWays; }; 
void SetShowFlatWays(bool value) { m_ShowFlatWays = value; }; 
bool GetDebugPointInRoad() { return m_DebugPointInRoad; }; 
void SetDebugPointInRoad(bool value) { m_DebugPointInRoad = value; }; 
bool GetCOMMAND_LoadAllDebugWays() { return m_COMMAND_LoadAllDebugWays; }; 
void SetCOMMAND_LoadAllDebugWays(bool value) { SetCOMMAND_LoadAllDebugWays_Callback(value); }; 
void SetCOMMAND_LoadAllDebugWays_Callback(const bool& value);
bool GetCOMMAND_CreateAllEntityModels() { return m_COMMAND_CreateAllEntityModels; }; 
void SetCOMMAND_CreateAllEntityModels(bool value) { SetCOMMAND_CreateAllEntityModels_Callback(value); }; 
void SetCOMMAND_CreateAllEntityModels_Callback(const bool& value);
private:
bool m_Enabled = true;
Node* m_SelectedNode = nullptr;
std::vector<Way*> m_SelectedWays;
OSMElementComponent* m_SelectedOSMElementComponent = nullptr;
bool m_SearchOnlyWithinSector = false;
glm::ivec2 m_SelectedSectorIndex = glm::ivec2(0, 0);
glm::vec3 m_WorldSpacePosition = glm::vec3(0, 0, 0);
bool m_ShowNodes = false;
bool m_ShowWays = false;
bool m_ShowFlatWays = true;
bool m_DebugPointInRoad = false;
bool m_COMMAND_LoadAllDebugWays = false;
bool m_COMMAND_CreateAllEntityModels = false;
public:
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMToolPIDX
{
PIDX_Enabled,
PIDX_SelectedNode,
PIDX_SelectedWays,
PIDX_SelectedOSMElementComponent,
PIDX_SearchOnlyWithinSector,
PIDX_SelectedSectorIndex,
PIDX_WorldSpacePosition,
PIDX_ShowNodes,
PIDX_ShowWays,
PIDX_ShowFlatWays,
PIDX_DebugPointInRoad,
PIDX_COMMAND_LoadAllDebugWays,
PIDX_COMMAND_CreateAllEntityModels,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMTool

OSMTool();

virtual void Reset();

bool Process(bool isActiveTool, bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;

void DebugDisplay() override;
void DisplaySectorDebug(Sector* sector);

std::vector<glm::vec3>& GetWayDebugColors() { return m_WayDebugColors; };
std::vector<int>& GetWayDisplayToggles() { return m_WayDisplayToggles; };

private:

std::vector<int> m_WayDisplayToggles;
std::vector<glm::vec3> m_WayDebugColors;
};
