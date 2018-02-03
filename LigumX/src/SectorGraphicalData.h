#pragma once

#pragma region  FORWARD_DECLARATIONS SectorGraphicalData
#include "property.h"

class SectorGraphicalData;
class Mesh;


#pragma endregion  FORWARD_DECLARATIONS SectorGraphicalData

class SectorGraphicalData
{
#pragma region  HEADER SectorGraphicalData
public:
static const int ClassID = 1523090688;
static const LXType Type = LXType_SectorGraphicalData;
static constexpr const char* ClassName = "SectorGraphicalData";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
Mesh*& GetNodesMesh() { return m_NodesMesh; }; 
void SetNodesMesh(Mesh* value) { m_NodesMesh = value; }; 
Mesh*& GetWaysMesh() { return m_WaysMesh; }; 
void SetWaysMesh(Mesh* value) { m_WaysMesh = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Mesh* m_NodesMesh;
Mesh* m_WaysMesh;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorGraphicalDataPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_NodesMesh,
PIDX_WaysMesh,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorGraphicalData

};
