#pragma once

#include "glm\glm.hpp"
#include "CurlRequest.h"
#include <thread>

class SectorGraphicalData;


#pragma region  FORWARD_DECLARATIONS SectorTool
#include "property.h"

class SectorTool;
class Sector;


#pragma endregion  FORWARD_DECLARATIONS SectorTool

class SectorTool
{
#pragma region  HEADER SectorTool
public:
static const int ClassID = 2610247209;
static const LXType Type = LXType_SectorTool;
static constexpr const char* ClassName = "SectorTool";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
const glm::vec3& GetHighlightedWorldCoordinates() { return m_HighlightedWorldCoordinates; }; 
void SetHighlightedWorldCoordinates(glm::vec3 value) { m_HighlightedWorldCoordinates = value; }; 
Sector*& GetHighlightedSector() { return m_HighlightedSector; }; 
void SetHighlightedSector(Sector* value) { m_HighlightedSector = value; }; 
const glm::vec3& GetSectorGridColor() { return m_SectorGridColor; }; 
void SetSectorGridColor(glm::vec3 value) { m_SectorGridColor = value; }; 
const bool& GetAsyncSectorLoading() { return m_AsyncSectorLoading; }; 
void SetAsyncSectorLoading(bool value) { m_AsyncSectorLoading = value; }; 
const float& GetNodeSize() { return m_NodeSize; }; 
void SetNodeSize(float value) { m_NodeSize = value; }; 
const bool& GetShowNodes() { return m_ShowNodes; }; 
void SetShowNodes(bool value) { m_ShowNodes = value; }; 
const bool& GetShowWays() { return m_ShowWays; }; 
void SetShowWays(bool value) { m_ShowWays = value; }; 
const bool& GetShowFlatWays() { return m_ShowFlatWays; }; 
void SetShowFlatWays(bool value) { m_ShowFlatWays = value; }; 
const bool& GetShowSectorAABBs() { return m_ShowSectorAABBs; }; 
void SetShowSectorAABBs(bool value) { m_ShowSectorAABBs = value; }; 
const bool& GetShowGrid() { return m_ShowGrid; }; 
void SetShowGrid(bool value) { m_ShowGrid = value; }; 
const bool& GetHighlightSelectedSector() { return m_HighlightSelectedSector; }; 
void SetHighlightSelectedSector(bool value) { m_HighlightSelectedSector = value; }; 
const bool& GetLoadSectorsOnClick() { return m_LoadSectorsOnClick; }; 
void SetLoadSectorsOnClick(bool value) { m_LoadSectorsOnClick = value; }; 
const int& GetSelectedWayIndex() { return m_SelectedWayIndex; }; 
void SetSelectedWayIndex(int value) { m_SelectedWayIndex = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = true;
glm::vec3 m_HighlightedWorldCoordinates = glm::vec3(0, 0, 0);
Sector* m_HighlightedSector;
glm::vec3 m_SectorGridColor = glm::vec3(0.5);
bool m_AsyncSectorLoading = false;
float m_NodeSize = 2;
bool m_ShowNodes = false;
bool m_ShowWays = false;
bool m_ShowFlatWays = true;
bool m_ShowSectorAABBs = true;
bool m_ShowGrid = true;
bool m_HighlightSelectedSector = true;
bool m_LoadSectorsOnClick = true;
int m_SelectedWayIndex = 0;
public:
static const int g_PropertyCount = 16;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_HighlightedWorldCoordinates,
PIDX_HighlightedSector,
PIDX_SectorGridColor,
PIDX_AsyncSectorLoading,
PIDX_NodeSize,
PIDX_ShowNodes,
PIDX_ShowWays,
PIDX_ShowFlatWays,
PIDX_ShowSectorAABBs,
PIDX_ShowGrid,
PIDX_HighlightSelectedSector,
PIDX_LoadSectorsOnClick,
PIDX_SelectedWayIndex,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorTool

SectorTool();


bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);


CurlRequest m_Request;

Sector* m_LoadingSector;

void DisplaySectorDebug(Sector* sector);
void Display();

glm::vec3 GetWorldSpaceRay(glm::vec3 ndc, const glm::mat4& projectionMatrixInverse, const glm::mat4& viewMatrixInverse);
glm::vec3 GetAimingWorldSpacePosition2(glm::vec3 worldSpaceRay, glm::vec3 cameraPosition);
glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mousePosition);


glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mouseScreenPosition, bool printDebugInfo);
glm::vec3 GetHighlightColor(Sector* sector);

std::vector<glm::vec3>& GetWayDebugColors() { return m_WayDebugColors; };
std::vector<int>& GetWayDisplayToggles() { return m_WayDisplayToggles; };
private:
	std::vector<int> m_WayDisplayToggles;
	std::vector<glm::vec3> m_WayDebugColors;

};
