#include "EngineSettings.h"

#pragma region  CLASS_SOURCE EngineSettings
EngineSettings* g_EngineSettings;

#include "EngineSettings.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EngineSettings::g_Properties[] = 
{
{ "WorldScale", PIDX_WorldScale, offsetof(EngineSettings, m_WorldScale), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "StartLonLat", PIDX_StartLonLat, offsetof(EngineSettings, m_StartLonLat), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Extent", PIDX_Extent, offsetof(EngineSettings, m_Extent), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "OSMQuantizationScale", PIDX_OSMQuantizationScale, offsetof(EngineSettings, m_OSMQuantizationScale), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "GenerateFlatTerrain", PIDX_GenerateFlatTerrain, offsetof(EngineSettings, m_GenerateFlatTerrain), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SplatMapResolution", PIDX_SplatMapResolution, offsetof(EngineSettings, m_SplatMapResolution), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "WayTessellationFactor", PIDX_WayTessellationFactor, offsetof(EngineSettings, m_WayTessellationFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MessagesStartingPosition", PIDX_MessagesStartingPosition, offsetof(EngineSettings, m_MessagesStartingPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "MessagesFontSize", PIDX_MessagesFontSize, offsetof(EngineSettings, m_MessagesFontSize), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MessagesDefaultFrameCount", PIDX_MessagesDefaultFrameCount, offsetof(EngineSettings, m_MessagesDefaultFrameCount), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "MessagesPixelsOffset", PIDX_MessagesPixelsOffset, offsetof(EngineSettings, m_MessagesPixelsOffset), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "LoadOSMData", PIDX_LoadOSMData, offsetof(EngineSettings, m_LoadOSMData), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "TerrainTiling", PIDX_TerrainTiling, offsetof(EngineSettings, m_TerrainTiling), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
bool EngineSettings::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EngineSettings::ShowPropertyGrid()
{
	LXIMGUI_SHOW_FLOAT("WorldScale", m_WorldScale, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_VEC2("StartLonLat", m_StartLonLat, 0, 0);
	LXIMGUI_SHOW_FLOAT("Extent", m_Extent, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_INT("OSMQuantizationScale", m_OSMQuantizationScale, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_BOOL("GenerateFlatTerrain", m_GenerateFlatTerrain);
	LXIMGUI_SHOW_FLOAT("WayTessellationFactor", m_WayTessellationFactor, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_VEC2("MessagesStartingPosition", m_MessagesStartingPosition, 0, 0);
	LXIMGUI_SHOW_FLOAT("MessagesFontSize", m_MessagesFontSize, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_INT("MessagesDefaultFrameCount", m_MessagesDefaultFrameCount, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_INT("MessagesPixelsOffset", m_MessagesPixelsOffset, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_BOOL("LoadOSMData", m_LoadOSMData);
	LXIMGUI_SHOW_FLOAT("TerrainTiling", m_TerrainTiling, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	return true;
}

#pragma endregion  CLASS_SOURCE EngineSettings

EngineSettings::EngineSettings()
{
}

EngineSettings::EngineSettings(int id)
{
	SetObjectID(id);

	Serialize(false);
}
