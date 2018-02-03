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
const bool& Geta() { return m_a; }; 
void Seta(bool value) { m_a = value; }; 
const bool& GetShowHIGHWAYTRUNK() { return m_ShowHIGHWAYTRUNK; }; 
void SetShowHIGHWAYTRUNK(bool value) { m_ShowHIGHWAYTRUNK = value; }; 
const bool& GetShowHIGHWAYPRIMARY() { return m_ShowHIGHWAYPRIMARY; }; 
void SetShowHIGHWAYPRIMARY(bool value) { m_ShowHIGHWAYPRIMARY = value; }; 
const bool& GetShowHIGHWAYSECONDARY() { return m_ShowHIGHWAYSECONDARY; }; 
void SetShowHIGHWAYSECONDARY(bool value) { m_ShowHIGHWAYSECONDARY = value; }; 
const bool& GetShowHIGHWAYTERTIARY() { return m_ShowHIGHWAYTERTIARY; }; 
void SetShowHIGHWAYTERTIARY(bool value) { m_ShowHIGHWAYTERTIARY = value; }; 
const bool& GetShowHIGHWAYRESIDENTIAL() { return m_ShowHIGHWAYRESIDENTIAL; }; 
void SetShowHIGHWAYRESIDENTIAL(bool value) { m_ShowHIGHWAYRESIDENTIAL = value; }; 
const bool& GetShowHIGHWAYUNCLASSIFIED() { return m_ShowHIGHWAYUNCLASSIFIED; }; 
void SetShowHIGHWAYUNCLASSIFIED(bool value) { m_ShowHIGHWAYUNCLASSIFIED = value; }; 
const bool& GetShowHIGHWAYSERVICE() { return m_ShowHIGHWAYSERVICE; }; 
void SetShowHIGHWAYSERVICE(bool value) { m_ShowHIGHWAYSERVICE = value; }; 
const bool& GetShowBUILDINGUNMARKED() { return m_ShowBUILDINGUNMARKED; }; 
void SetShowBUILDINGUNMARKED(bool value) { m_ShowBUILDINGUNMARKED = value; }; 
const bool& GetShowBUILDINGSCHOOL() { return m_ShowBUILDINGSCHOOL; }; 
void SetShowBUILDINGSCHOOL(bool value) { m_ShowBUILDINGSCHOOL = value; }; 
const bool& GetShowBUILDINGADDRINTERP() { return m_ShowBUILDINGADDRINTERP; }; 
void SetShowBUILDINGADDRINTERP(bool value) { m_ShowBUILDINGADDRINTERP = value; }; 
const bool& GetShowRAILWAYSUBWAY() { return m_ShowRAILWAYSUBWAY; }; 
void SetShowRAILWAYSUBWAY(bool value) { m_ShowRAILWAYSUBWAY = value; }; 
const bool& GetShowNATURALWOOD() { return m_ShowNATURALWOOD; }; 
void SetShowNATURALWOOD(bool value) { m_ShowNATURALWOOD = value; }; 
const bool& GetShowNATURALWATER() { return m_ShowNATURALWATER; }; 
void SetShowNATURALWATER(bool value) { m_ShowNATURALWATER = value; }; 
const bool& GetShowLEISUREPARK() { return m_ShowLEISUREPARK; }; 
void SetShowLEISUREPARK(bool value) { m_ShowLEISUREPARK = value; }; 
const bool& GetShowADDRINTERPOLATION() { return m_ShowADDRINTERPOLATION; }; 
void SetShowADDRINTERPOLATION(bool value) { m_ShowADDRINTERPOLATION = value; }; 
const bool& GetShowGRIDLINE() { return m_ShowGRIDLINE; }; 
void SetShowGRIDLINE(bool value) { m_ShowGRIDLINE = value; }; 
const bool& GetShowaDEBUG() { return m_ShowaDEBUG; }; 
void SetShowaDEBUG(bool value) { m_ShowaDEBUG = value; }; 
const bool& GetShowLANDUSE() { return m_ShowLANDUSE; }; 
void SetShowLANDUSE(bool value) { m_ShowLANDUSE = value; }; 
const bool& GetShowBOUNDARY() { return m_ShowBOUNDARY; }; 
void SetShowBOUNDARY(bool value) { m_ShowBOUNDARY = value; }; 
const bool& GetShowCONTOUR() { return m_ShowCONTOUR; }; 
void SetShowCONTOUR(bool value) { m_ShowCONTOUR = value; }; 
const int& GetWayDisplayFlags() { return m_WayDisplayFlags; }; 
void SetWayDisplayFlags(int value) { m_WayDisplayFlags = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = true;
glm::vec3 m_HighlightedWorldCoordinates = glm::vec3(0, 0, 0);
Sector* m_HighlightedSector;
glm::vec3 m_SectorGridColor = glm::vec3(0.5);
bool m_AsyncSectorLoading = false;
float m_NodeSize = 2;
bool m_ShowNodes = true;
bool m_ShowWays = true;
bool m_ShowFlatWays = false;
bool m_ShowSectorAABBs = true;
bool m_ShowGrid = true;
bool m_HighlightSelectedSector = true;
bool m_LoadSectorsOnClick = true;
bool m_a = true;
bool m_ShowHIGHWAYTRUNK = true;
bool m_ShowHIGHWAYPRIMARY = true;
bool m_ShowHIGHWAYSECONDARY = true;
bool m_ShowHIGHWAYTERTIARY = true;
bool m_ShowHIGHWAYRESIDENTIAL = true;
bool m_ShowHIGHWAYUNCLASSIFIED = true;
bool m_ShowHIGHWAYSERVICE = true;
bool m_ShowBUILDINGUNMARKED = true;
bool m_ShowBUILDINGSCHOOL = true;
bool m_ShowBUILDINGADDRINTERP = true;
bool m_ShowRAILWAYSUBWAY = true;
bool m_ShowNATURALWOOD = true;
bool m_ShowNATURALWATER = true;
bool m_ShowLEISUREPARK = true;
bool m_ShowADDRINTERPOLATION = true;
bool m_ShowGRIDLINE = true;
bool m_ShowaDEBUG = true;
bool m_ShowLANDUSE = true;
bool m_ShowBOUNDARY = true;
bool m_ShowCONTOUR = true;
int m_WayDisplayFlags = 0;
public:
static const int g_PropertyCount = 37;
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
PIDX_a,
PIDX_ShowHIGHWAYTRUNK,
PIDX_ShowHIGHWAYPRIMARY,
PIDX_ShowHIGHWAYSECONDARY,
PIDX_ShowHIGHWAYTERTIARY,
PIDX_ShowHIGHWAYRESIDENTIAL,
PIDX_ShowHIGHWAYUNCLASSIFIED,
PIDX_ShowHIGHWAYSERVICE,
PIDX_ShowBUILDINGUNMARKED,
PIDX_ShowBUILDINGSCHOOL,
PIDX_ShowBUILDINGADDRINTERP,
PIDX_ShowRAILWAYSUBWAY,
PIDX_ShowNATURALWOOD,
PIDX_ShowNATURALWATER,
PIDX_ShowLEISUREPARK,
PIDX_ShowADDRINTERPOLATION,
PIDX_ShowGRIDLINE,
PIDX_ShowaDEBUG,
PIDX_ShowLANDUSE,
PIDX_ShowBOUNDARY,
PIDX_ShowCONTOUR,
PIDX_WayDisplayFlags,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorTool

bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);


CurlRequest m_Request;

Sector* m_LoadingSector;

void DisplaySectorDebug(SectorGraphicalData* gfxData);
void Display();

glm::vec3 GetWorldSpaceRay(glm::vec3 ndc, const glm::mat4& projectionMatrixInverse, const glm::mat4& viewMatrixInverse);
glm::vec3 GetAimingWorldSpacePosition2(glm::vec3 worldSpaceRay, glm::vec3 cameraPosition);
glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mousePosition);


glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mouseScreenPosition, bool printDebugInfo);
glm::vec3 GetHighlightColor(Sector* sector);


};
