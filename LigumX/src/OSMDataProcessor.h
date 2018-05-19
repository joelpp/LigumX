#pragma once

#include "glm/glm.hpp"


class Mesh;
class Way;
class Sector;
class OSMDataProcessorSettings;

#pragma region  FORWARD_DECLARATIONS OSMDataProcessor
#include "property.h"

class OSMDataProcessor;
class OSMDataProcessorSettings;

extern OSMDataProcessor* g_OSMDataProcessor;

#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessor

bool PointInRoad(Sector* sector, const glm::vec3& worldSpacePosition);

class OSMDataProcessor
{
#pragma region  HEADER OSMDataProcessor
public:
static const int ClassID = 3101510658;
static const LXType Type = LXType_OSMDataProcessor;
static constexpr const char* ClassName = "OSMDataProcessor";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const float& GetRoadWidth() { return m_RoadWidth; }; 
void SetRoadWidth(float value) { m_RoadWidth = value; }; 
OSMDataProcessorSettings*& GetSettings() { return m_Settings; }; 
void SetSettings(OSMDataProcessorSettings* value) { m_Settings = value; }; 
private:
int m_ObjectID;
std::string m_Name;
float m_RoadWidth = 0.f;
OSMDataProcessorSettings* m_Settings = nullptr;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMDataProcessorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_RoadWidth,
PIDX_Settings,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMDataProcessor

OSMDataProcessor();

void ProcessSector(Sector* sector);

void ProcessRoad(Sector* sector, Way* way);
Mesh* BuildRoadMesh(Sector* sector, Way* way);

Mesh* BuildAdressInterpolationBuilding(Sector* sector, Way* way);
void ProcessAddressInterpolation(Sector* sector, Way* way);
Mesh* BuildGenericBuilding(Sector* sector, Way* way);

void ProcessGenericBuilding(Sector* sector, Way* way);

static bool IsRoad(Way* way);

private:
};
