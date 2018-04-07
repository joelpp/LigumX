#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "EditorTool.h"

class Texture;

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
const TerrainEditionMode& GetMode() { return m_Mode; }; 
void SetMode(TerrainEditionMode value) { m_Mode = value; }; 
const bool& GetTerrainErasureMode() { return m_TerrainErasureMode; }; 
void SetTerrainErasureMode(bool value) { m_TerrainErasureMode = value; }; 
const float& GetTerrainBrushSize() { return m_TerrainBrushSize; }; 
void SetTerrainBrushSize(float value) { m_TerrainBrushSize = value; }; 
void AddTo_TerrainBrushSize(float value) { m_TerrainBrushSize += value; };
const glm::vec4& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(glm::vec4 value) { m_XYZMask = value; }; 
const glm::ivec2& GetClickedTexel() { return m_ClickedTexel; }; 
void SetClickedTexel(glm::ivec2 value) { m_ClickedTexel = value; }; 
const glm::ivec2& GetStartTexel() { return m_StartTexel; }; 
void SetStartTexel(glm::ivec2 value) { m_StartTexel = value; }; 
const int& GetSplatMapIncrement() { return m_SplatMapIncrement; }; 
void SetSplatMapIncrement(int value) { m_SplatMapIncrement = value; }; 
Sector*& GetClickedSector() { return m_ClickedSector; }; 
void SetClickedSector(Sector* value) { m_ClickedSector = value; }; 
private:
int m_ObjectID;
std::string m_Name;
TerrainEditionMode m_Mode;
bool m_TerrainErasureMode = false;
float m_TerrainBrushSize = 10;
glm::vec4 m_XYZMask;
glm::ivec2 m_ClickedTexel = glm::ivec2(0, 0);
glm::ivec2 m_StartTexel = glm::ivec2(0, 0);
int m_SplatMapIncrement = 10;
Sector* m_ClickedSector;
public:
static const int g_PropertyCount = 10;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TerrainToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Mode,
PIDX_TerrainErasureMode,
PIDX_TerrainBrushSize,
PIDX_XYZMask,
PIDX_ClickedTexel,
PIDX_StartTexel,
PIDX_SplatMapIncrement,
PIDX_ClickedSector,
};
bool Serialize(bool writing);

#pragma endregion  HEADER TerrainTool

public:

	TerrainTool();

	bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;
	void HandleKeyboardInput(int button, int action, int mods) override;
	bool HandleMouseScroll(const glm::vec2& scrolling) override;

	Texture* GetTextureToDisplay();

private:

};
