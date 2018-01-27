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
};
bool EngineSettings::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE EngineSettings


EngineSettings::EngineSettings(int id)
{
	m_ObjectID = id;

	Serialize(false);
}
