#include "EngineStats.h"

#pragma region  CLASS_SOURCE EngineStats
EngineStats* g_EngineStats;

#include "EngineStats.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EngineStats::g_Properties[] = 
{
{ "NumObjectMapHits", PIDX_NumObjectMapHits, offsetof(EngineStats, m_NumObjectMapHits), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
{ "NumDrawCalls", PIDX_NumDrawCalls, offsetof(EngineStats, m_NumDrawCalls), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
};
bool EngineStats::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
void EngineStats::ShowPropertyGrid()
{
	LXIMGUI_SHOW_INT("NumObjectMapHits", m_NumObjectMapHits);
	LXIMGUI_SHOW_INT("NumDrawCalls", m_NumDrawCalls);
}

#pragma endregion  CLASS_SOURCE EngineStats


void EngineStats::ResetFrame()
{
	m_NumDrawCalls = 0;
}
