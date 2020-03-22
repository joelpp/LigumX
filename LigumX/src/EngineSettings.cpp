#include "EngineSettings.h"

#pragma region  CLASS_SOURCE EngineSettings
EngineSettings* g_EngineSettings;

#include "EngineSettings.h"
#include "serializer.h"
const ClassPropertyData EngineSettings::g_Properties[] = 
{
{ "WorldScale", PIDX_WorldScale, offsetof(EngineSettings, m_WorldScale), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "StartLonLat", PIDX_StartLonLat, offsetof(EngineSettings, m_StartLonLat), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Extent", PIDX_Extent, offsetof(EngineSettings, m_Extent), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "OSMQuantizationScale", PIDX_OSMQuantizationScale, offsetof(EngineSettings, m_OSMQuantizationScale), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "GenerateFlatTerrain", PIDX_GenerateFlatTerrain, offsetof(EngineSettings, m_GenerateFlatTerrain), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SplatMapResolution", PIDX_SplatMapResolution, offsetof(EngineSettings, m_SplatMapResolution), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "WayTessellationFactor", PIDX_WayTessellationFactor, offsetof(EngineSettings, m_WayTessellationFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MessagesStartingPosition", PIDX_MessagesStartingPosition, offsetof(EngineSettings, m_MessagesStartingPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MessagesFontSize", PIDX_MessagesFontSize, offsetof(EngineSettings, m_MessagesFontSize), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "MessagesDefaultFrameCount", PIDX_MessagesDefaultFrameCount, offsetof(EngineSettings, m_MessagesDefaultFrameCount), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "MessagesPixelsOffset", PIDX_MessagesPixelsOffset, offsetof(EngineSettings, m_MessagesPixelsOffset), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "LoadOSMData", PIDX_LoadOSMData, offsetof(EngineSettings, m_LoadOSMData), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "TerrainTiling", PIDX_TerrainTiling, offsetof(EngineSettings, m_TerrainTiling), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
void EngineSettings::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeFloat(g_Properties[PIDX_WorldScale], m_WorldScale);
	serializer.SerializeVec2(g_Properties[PIDX_StartLonLat], m_StartLonLat);
	serializer.SerializeFloat(g_Properties[PIDX_Extent], m_Extent);
	serializer.SerializeInt(g_Properties[PIDX_OSMQuantizationScale], m_OSMQuantizationScale);
	serializer.SerializeBool(g_Properties[PIDX_GenerateFlatTerrain], m_GenerateFlatTerrain);
	serializer.SerializeIVec2(g_Properties[PIDX_SplatMapResolution], m_SplatMapResolution);
	serializer.SerializeFloat(g_Properties[PIDX_WayTessellationFactor], m_WayTessellationFactor);
	serializer.SerializeVec2(g_Properties[PIDX_MessagesStartingPosition], m_MessagesStartingPosition);
	serializer.SerializeFloat(g_Properties[PIDX_MessagesFontSize], m_MessagesFontSize);
	serializer.SerializeInt(g_Properties[PIDX_MessagesDefaultFrameCount], m_MessagesDefaultFrameCount);
	serializer.SerializeInt(g_Properties[PIDX_MessagesPixelsOffset], m_MessagesPixelsOffset);
	serializer.SerializeBool(g_Properties[PIDX_LoadOSMData], m_LoadOSMData);
	serializer.SerializeFloat(g_Properties[PIDX_TerrainTiling], m_TerrainTiling);
	serializer.Close();
}
bool EngineSettings::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EngineSettings::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowFloat("WorldScale", m_WorldScale , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowVec2("StartLonLat", m_StartLonLat , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("Extent", m_Extent , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowInt("OSMQuantizationScale", m_OSMQuantizationScale , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowBool("GenerateFlatTerrain", m_GenerateFlatTerrain  );
	ImguiHelpers::ShowIVec2("SplatMapResolution", m_SplatMapResolution , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowFloat("WayTessellationFactor", m_WayTessellationFactor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowVec2("MessagesStartingPosition", m_MessagesStartingPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowFloat("MessagesFontSize", m_MessagesFontSize , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowInt("MessagesDefaultFrameCount", m_MessagesDefaultFrameCount , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowInt("MessagesPixelsOffset", m_MessagesPixelsOffset , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowBool("LoadOSMData", m_LoadOSMData  );
	ImguiHelpers::ShowFloat("TerrainTiling", m_TerrainTiling , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
const char* EngineSettings::GetTypeName()
{
	return ClassName;
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
