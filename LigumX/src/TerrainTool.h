#pragma once

#include "glm/glm.hpp"
#include "EditorTool.h"


#pragma region  FORWARD_DECLARATIONS TerrainTool
#include "property.h"

class TerrainTool;
class Sector;

enum TerrainEditionMode
{
TerrainEditionMode_None,
TerrainEditionMode_Height,
TerrainEditionMode_Color,
};

extern const std::string EnumValues_TerrainEditionMode[3];
extern const TerrainEditionMode Indirection_TerrainEditionMode[3];
const int EnumLength_TerrainEditionMode = 3;


#pragma endregion  FORWARD_DECLARATIONS TerrainTool

class TerrainTool : public EditorTool
{
#pragma region  HEADER TerrainTool
public:
static const int ClassID = 2789929640;
static const LXType Type = LXType_TerrainTool;
static constexpr const char* ClassName = "TerrainTool";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
Sector*& GetSector() { return m_Sector; }; 
void SetSector(Sector* value) { m_Sector = value; }; 
const TerrainEditionMode& GetMode() { return m_Mode; }; 
void SetMode(TerrainEditionMode value) { m_Mode = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Sector* m_Sector;
TerrainEditionMode m_Mode;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TerrainToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Sector,
PIDX_Mode,
};
bool Serialize(bool writing);

#pragma endregion  HEADER TerrainTool

public:

	bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;


};
