#include "EngineStats.h"

#pragma region  CLASS_SOURCE EngineStats
EngineStats* g_EngineStats;

#include "EngineStats.h"
const ClassPropertyData EngineStats::g_Properties[] = 
{
{ "NumObjectMapHits", PIDX_NumObjectMapHits, offsetof(EngineStats, m_NumObjectMapHits), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "NumDrawCalls", PIDX_NumDrawCalls, offsetof(EngineStats, m_NumDrawCalls), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
};
bool EngineStats::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EngineStats::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_INT("NumObjectMapHits", m_NumObjectMapHits, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_INT("NumDrawCalls", m_NumDrawCalls, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	return true;
}
const char* EngineStats::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE EngineStats


void EngineStats::ResetFrame()
{
	m_NumDrawCalls = 0;
}
