#pragma once

class Way;
class Sector;

#pragma region  FORWARD_DECLARATIONS OSMDataProcessor
#include "property.h"

class OSMDataProcessor;

extern OSMDataProcessor* g_OSMDataProcessor;

#pragma endregion  FORWARD_DECLARATIONS OSMDataProcessor

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
private:
int m_ObjectID;
std::string m_Name;
float m_RoadWidth = 0.f;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMDataProcessorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_RoadWidth,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMDataProcessor

void ProcessSector(Sector* sector);

void ProcessRoad(Sector* sector, Way* way);
void BuildRoadMeshes(Sector* sector, Way* way);



};
