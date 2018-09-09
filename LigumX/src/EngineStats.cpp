#include "EngineStats.h"

#pragma region  CLASS_SOURCE EngineStats
EngineStats* g_EngineStats;

#include "EngineStats.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EngineStats::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(EngineStats, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(EngineStats, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "NumObjectMapHits", PIDX_NumObjectMapHits, offsetof(EngineStats, m_NumObjectMapHits), 0, LXType_int, false, LXType_None, PropertyFlags_Adder, 0, 0, 0,}, 
{ "NumDrawCalls", PIDX_NumDrawCalls, offsetof(EngineStats, m_NumDrawCalls), 0, LXType_int, false, LXType_None, PropertyFlags_Adder, 0, 0, 0,}, 
};
bool EngineStats::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE EngineStats


void EngineStats::ResetFrame()
{
	m_NumDrawCalls = 0;
}
