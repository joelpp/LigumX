#pragma once


class Texture;

#pragma region  FORWARD_DECLARATIONS TerrainTool
#include "LXSystem.h"
class Serializer2;

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

#include "EditorTool.h"


#pragma region  HEADER TerrainTool
class TerrainTool : public EditorTool
{
public:
static const int ClassID = 1369394216;
static const LXType Type = LXType_TerrainTool;
static constexpr const char* ClassName = "TerrainTool";
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

const TerrainEditionMode& GetMode() { return m_Mode; }; 
void SetMode(TerrainEditionMode value) { m_Mode = value; }; 
bool GetTerrainErasureMode() { return m_TerrainErasureMode; }; 
void SetTerrainErasureMode(bool value) { m_TerrainErasureMode = value; }; 
float GetTerrainBrushSize() { return m_TerrainBrushSize; }; 
void SetTerrainBrushSize(float value) { m_TerrainBrushSize = value; }; 
void AddTo_TerrainBrushSize(float value) { m_TerrainBrushSize += value; };
glm::vec4& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(const glm::vec4& value) { m_XYZMask = value; }; 
glm::ivec2& GetClickedTexel() { return m_ClickedTexel; }; 
void SetClickedTexel(const glm::ivec2& value) { m_ClickedTexel = value; }; 
glm::ivec2& GetStartTexel() { return m_StartTexel; }; 
void SetStartTexel(const glm::ivec2& value) { m_StartTexel = value; }; 
int GetSplatMapIncrement() { return m_SplatMapIncrement; }; 
void SetSplatMapIncrement(int value) { m_SplatMapIncrement = value; }; 
Sector*& GetClickedSector() { return m_ClickedSector; }; 
void SetClickedSector(Sector* value) { m_ClickedSector = value; }; 
private:
TerrainEditionMode m_Mode;
bool m_TerrainErasureMode = false;
float m_TerrainBrushSize = 10;
glm::vec4 m_XYZMask;
glm::ivec2 m_ClickedTexel = glm::ivec2(0, 0);
glm::ivec2 m_StartTexel = glm::ivec2(0, 0);
int m_SplatMapIncrement = 10;
Sector* m_ClickedSector = nullptr;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_TerrainToolPIDX
{
PIDX_Mode,
PIDX_TerrainErasureMode,
PIDX_TerrainBrushSize,
PIDX_XYZMask,
PIDX_ClickedTexel,
PIDX_StartTexel,
PIDX_SplatMapIncrement,
PIDX_ClickedSector,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(TerrainTool& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER TerrainTool

public:

	TerrainTool();

	bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;
	void HandleKeyboardInput(int button, int action, int mods) override;
	bool HandleMouseScroll(const glm::vec2& scrolling) override;

	Texture* GetTextureToDisplay();

private:

};
