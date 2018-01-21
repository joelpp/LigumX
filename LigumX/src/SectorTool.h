#pragma once

#include "glm\glm.hpp"
#include "CurlRequest.h"
#include <thread>


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
const glm::vec3& GetColorNullSector() { return m_ColorNullSector; }; 
void SetColorNullSector(glm::vec3 value) { m_ColorNullSector = value; }; 
const glm::vec3& GetColorUnloadedSector() { return m_ColorUnloadedSector; }; 
void SetColorUnloadedSector(glm::vec3 value) { m_ColorUnloadedSector = value; }; 
const glm::vec3& GetColorLoadedSector() { return m_ColorLoadedSector; }; 
void SetColorLoadedSector(glm::vec3 value) { m_ColorLoadedSector = value; }; 
private:
int m_ObjectID
;
std::string m_Name
;
bool m_Enabled
 = false;
glm::vec3 m_HighlightedWorldCoordinates
 = glm::vec3(0, 0, 0);
Sector* m_HighlightedSector
;
glm::vec3 m_ColorNullSector
 = glm::vec3(010);
glm::vec3 m_ColorUnloadedSector
 = glm::vec3(011);
glm::vec3 m_ColorLoadedSector
 = glm::vec3(001);
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_HighlightedWorldCoordinates,
PIDX_HighlightedSector,
PIDX_ColorNullSector,
PIDX_ColorUnloadedSector,
PIDX_ColorLoadedSector,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorTool

bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);


CurlRequest m_Request;

Sector* m_LoadingSector;

void Display();

glm::vec3 GetAimingWorldSpacePosition(const glm::vec2& mouseScreenPosition, bool printDebugInfo);
glm::vec3 GetHighlightColor(Sector* sector);


};
