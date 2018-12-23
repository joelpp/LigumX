#pragma once

#include "glm\glm.hpp"
#pragma region  FORWARD_DECLARATIONS EngineSettings
#include "LXSystem.h"

class EngineSettings;

extern EngineSettings* g_EngineSettings;

#pragma endregion  FORWARD_DECLARATIONS EngineSettings

#pragma region  HEADER EngineSettings
class EngineSettings : public LXObject
{
public:
static const int ClassID = 513733670;
static const LXType Type = LXType_EngineSettings;
static constexpr const char* ClassName = "EngineSettings";
typedef LXObject super;

float GetWorldScale() { return m_WorldScale; }; 
void SetWorldScale(float value) { m_WorldScale = value; }; 
glm::vec2& GetStartLonLat() { return m_StartLonLat; }; 
void SetStartLonLat(const glm::vec2& value) { m_StartLonLat = value; }; 
float GetExtent() { return m_Extent; }; 
void SetExtent(float value) { m_Extent = value; }; 
int GetOSMQuantizationScale() { return m_OSMQuantizationScale; }; 
void SetOSMQuantizationScale(int value) { m_OSMQuantizationScale = value; }; 
bool GetGenerateFlatTerrain() { return m_GenerateFlatTerrain; }; 
void SetGenerateFlatTerrain(bool value) { m_GenerateFlatTerrain = value; }; 
glm::ivec2& GetSplatMapResolution() { return m_SplatMapResolution; }; 
void SetSplatMapResolution(const glm::ivec2& value) { m_SplatMapResolution = value; }; 
float GetWayTessellationFactor() { return m_WayTessellationFactor; }; 
void SetWayTessellationFactor(float value) { m_WayTessellationFactor = value; }; 
glm::vec2& GetMessagesStartingPosition() { return m_MessagesStartingPosition; }; 
void SetMessagesStartingPosition(const glm::vec2& value) { m_MessagesStartingPosition = value; }; 
float GetMessagesFontSize() { return m_MessagesFontSize; }; 
void SetMessagesFontSize(float value) { m_MessagesFontSize = value; }; 
int GetMessagesDefaultFrameCount() { return m_MessagesDefaultFrameCount; }; 
void SetMessagesDefaultFrameCount(int value) { m_MessagesDefaultFrameCount = value; }; 
int GetMessagesPixelsOffset() { return m_MessagesPixelsOffset; }; 
void SetMessagesPixelsOffset(int value) { m_MessagesPixelsOffset = value; }; 
bool GetLoadOSMData() { return m_LoadOSMData; }; 
void SetLoadOSMData(bool value) { m_LoadOSMData = value; }; 
float GetTerrainTiling() { return m_TerrainTiling; }; 
void SetTerrainTiling(float value) { m_TerrainTiling = value; }; 
private:
float m_WorldScale = 0.f;
glm::vec2 m_StartLonLat = glm::vec2(0, 0);
float m_Extent = 0.f;
int m_OSMQuantizationScale = 1000000;
bool m_GenerateFlatTerrain = false;
glm::ivec2 m_SplatMapResolution = glm::ivec2(0, 0);
float m_WayTessellationFactor = 50.f;
glm::vec2 m_MessagesStartingPosition = glm::vec2(0, 0);
float m_MessagesFontSize = 1.f;
int m_MessagesDefaultFrameCount = 50;
int m_MessagesPixelsOffset = 10;
bool m_LoadOSMData = true;
float m_TerrainTiling = 1.f;
public:
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EngineSettingsPIDX
{
PIDX_WorldScale,
PIDX_StartLonLat,
PIDX_Extent,
PIDX_OSMQuantizationScale,
PIDX_GenerateFlatTerrain,
PIDX_SplatMapResolution,
PIDX_WayTessellationFactor,
PIDX_MessagesStartingPosition,
PIDX_MessagesFontSize,
PIDX_MessagesDefaultFrameCount,
PIDX_MessagesPixelsOffset,
PIDX_LoadOSMData,
PIDX_TerrainTiling,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER EngineSettings

EngineSettings();
EngineSettings(int id);


};
