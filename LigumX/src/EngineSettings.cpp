#include "EngineSettings.h"

#pragma region  CLASS_SOURCE EngineSettings
EngineSettings* g_EngineSettings;

#include "EngineSettings.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EngineSettings::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(EngineSettings, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(EngineSettings, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "WorldScale", PIDX_WorldScale, offsetof(EngineSettings, m_WorldScale), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "StartLonLat", PIDX_StartLonLat, offsetof(EngineSettings, m_StartLonLat), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "Extent", PIDX_Extent, offsetof(EngineSettings, m_Extent), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "OSMQuantizationScale", PIDX_OSMQuantizationScale, offsetof(EngineSettings, m_OSMQuantizationScale), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "GenerateFlatTerrain", PIDX_GenerateFlatTerrain, offsetof(EngineSettings, m_GenerateFlatTerrain), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SplatMapResolution", PIDX_SplatMapResolution, offsetof(EngineSettings, m_SplatMapResolution), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "WayTessellationFactor", PIDX_WayTessellationFactor, offsetof(EngineSettings, m_WayTessellationFactor), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "MessagesStartingPosition", PIDX_MessagesStartingPosition, offsetof(EngineSettings, m_MessagesStartingPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "MessagesFontSize", PIDX_MessagesFontSize, offsetof(EngineSettings, m_MessagesFontSize), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "MessagesDefaultFrameCount", PIDX_MessagesDefaultFrameCount, offsetof(EngineSettings, m_MessagesDefaultFrameCount), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "MessagesPixelsOffset", PIDX_MessagesPixelsOffset, offsetof(EngineSettings, m_MessagesPixelsOffset), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "LoadOSMData", PIDX_LoadOSMData, offsetof(EngineSettings, m_LoadOSMData), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "CullSectors", PIDX_CullSectors, offsetof(EngineSettings, m_CullSectors), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "CullEntities", PIDX_CullEntities, offsetof(EngineSettings, m_CullEntities), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "TerrainTiling", PIDX_TerrainTiling, offsetof(EngineSettings, m_TerrainTiling), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
};
bool EngineSettings::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE EngineSettings

EngineSettings::EngineSettings()
{
}

EngineSettings::EngineSettings(int id)
{
	m_ObjectID = id;

	Serialize(false);
}
