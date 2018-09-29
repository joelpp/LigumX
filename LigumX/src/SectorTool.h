#pragma once

#include "glm\glm.hpp"

#include "CurlRequest.h"
#include "EditorTool.h"

#include <thread>

class SectorGraphicalData;


#pragma region  FORWARD_DECLARATIONS SectorTool
#include "property.h"

class SectorTool;
class Sector;


#pragma endregion  FORWARD_DECLARATIONS SectorTool

class SectorTool : public EditorTool
{
#pragma region  HEADER SectorTool
public:
static const int ClassID = 2610247209;
static const LXType Type = LXType_SectorTool;
static constexpr const char* ClassName = "SectorTool";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
glm::vec3& GetHighlightedWorldCoordinates() { return m_HighlightedWorldCoordinates; }; 
void SetHighlightedWorldCoordinates(const glm::vec3& value) { m_HighlightedWorldCoordinates = value; }; 
glm::vec2& GetHighlightedSectorUV() { return m_HighlightedSectorUV; }; 
void SetHighlightedSectorUV(const glm::vec2& value) { m_HighlightedSectorUV = value; }; 
Sector* GetHighlightedSector() { return m_HighlightedSector; }; 
void SetHighlightedSector(Sector* value) { m_HighlightedSector = value; }; 
glm::vec3& GetSectorGridColor() { return m_SectorGridColor; }; 
void SetSectorGridColor(const glm::vec3& value) { m_SectorGridColor = value; }; 
bool GetAsyncSectorLoading() { return m_AsyncSectorLoading; }; 
void SetAsyncSectorLoading(bool value) { m_AsyncSectorLoading = value; }; 
float GetNodeSize() { return m_NodeSize; }; 
void SetNodeSize(float value) { m_NodeSize = value; }; 
int GetLoadingRingSize() { return m_LoadingRingSize; }; 
void SetLoadingRingSize(int value) { m_LoadingRingSize = value; }; 
bool GetShowSectorAABBs() { return m_ShowSectorAABBs; }; 
void SetShowSectorAABBs(bool value) { m_ShowSectorAABBs = value; }; 
bool GetShowGrid() { return m_ShowGrid; }; 
void SetShowGrid(bool value) { m_ShowGrid = value; }; 
bool GetHighlightSelectedSector() { return m_HighlightSelectedSector; }; 
void SetHighlightSelectedSector(bool value) { m_HighlightSelectedSector = value; }; 
bool GetLoadSectorsOnClick() { return m_LoadSectorsOnClick; }; 
void SetLoadSectorsOnClick(bool value) { m_LoadSectorsOnClick = value; }; 
int GetSelectedWayIndex() { return m_SelectedWayIndex; }; 
void SetSelectedWayIndex(int value) { m_SelectedWayIndex = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = true;
glm::vec3 m_HighlightedWorldCoordinates = glm::vec3(0, 0, 0);
glm::vec2 m_HighlightedSectorUV = glm::vec2(0, 0);
Sector* m_HighlightedSector = nullptr;
glm::vec3 m_SectorGridColor = glm::vec3(0.5);
bool m_AsyncSectorLoading = false;
float m_NodeSize = 2;
int m_LoadingRingSize = 1;
bool m_ShowSectorAABBs = true;
bool m_ShowGrid = true;
bool m_HighlightSelectedSector = true;
bool m_LoadSectorsOnClick = true;
int m_SelectedWayIndex = 0;
public:
static const int g_PropertyCount = 15;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_HighlightedWorldCoordinates,
PIDX_HighlightedSectorUV,
PIDX_HighlightedSector,
PIDX_SectorGridColor,
PIDX_AsyncSectorLoading,
PIDX_NodeSize,
PIDX_LoadingRingSize,
PIDX_ShowSectorAABBs,
PIDX_ShowGrid,
PIDX_HighlightSelectedSector,
PIDX_LoadSectorsOnClick,
PIDX_SelectedWayIndex,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorTool

SectorTool();


virtual bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;

CurlRequest m_Request;

Sector* m_LoadingSector;

void DebugDisplay() override;

glm::vec3 GetHighlightColor(Sector* sector);
bool HandleMouseScroll(const glm::vec2& scrolling) override;


};
