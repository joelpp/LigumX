#pragma once

#include "glm\glm.hpp"
#pragma region  FORWARD_DECLARATIONS EngineSettings
#include "property.h"

class EngineSettings;

extern EngineSettings* g_EngineSettings;

#pragma endregion  FORWARD_DECLARATIONS EngineSettings

class EngineSettings
{
#pragma region  HEADER EngineSettings
public:
static const int ClassID = 513733670;
static const LXType Type = LXType_EngineSettings;
static constexpr const char* ClassName = "EngineSettings";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const float& GetWorldScale() { return m_WorldScale; }; 
void SetWorldScale(float value) { m_WorldScale = value; }; 
const glm::vec2& GetStartLonLat() { return m_StartLonLat; }; 
void SetStartLonLat(glm::vec2 value) { m_StartLonLat = value; }; 
const float& GetExtent() { return m_Extent; }; 
void SetExtent(float value) { m_Extent = value; }; 
const int& GetOSMQuantizationScale() { return m_OSMQuantizationScale; }; 
void SetOSMQuantizationScale(int value) { m_OSMQuantizationScale = value; }; 
const bool& GetGenerateFlatTerrain() { return m_GenerateFlatTerrain; }; 
void SetGenerateFlatTerrain(bool value) { m_GenerateFlatTerrain = value; }; 
const float& GetWayTessellationFactor() { return m_WayTessellationFactor; }; 
void SetWayTessellationFactor(float value) { m_WayTessellationFactor = value; }; 
private:
int m_ObjectID;
std::string m_Name;
float m_WorldScale = 0.f;
glm::vec2 m_StartLonLat = glm::vec2(0, 0);
float m_Extent = 0.f;
int m_OSMQuantizationScale = 1000000;
bool m_GenerateFlatTerrain = false;
float m_WayTessellationFactor = 1.0f;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EngineSettingsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_WorldScale,
PIDX_StartLonLat,
PIDX_Extent,
PIDX_OSMQuantizationScale,
PIDX_GenerateFlatTerrain,
PIDX_WayTessellationFactor,
};
bool Serialize(bool writing);

#pragma endregion  HEADER EngineSettings

EngineSettings();
EngineSettings(int id);


};
