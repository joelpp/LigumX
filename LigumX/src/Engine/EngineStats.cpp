#include "EngineStats.h"

#pragma region  CLASS_SOURCE EngineStats
EngineStats* g_EngineStats;

#include "EngineStats.h"
#include "serializer.h"
const ClassPropertyData EngineStats::g_Properties[] = 
{
{ "NumObjectMapHits", PIDX_NumObjectMapHits, offsetof(EngineStats, m_NumObjectMapHits), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "NumDrawCalls", PIDX_NumDrawCalls, offsetof(EngineStats, m_NumDrawCalls), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
};
void EngineStats::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_NumObjectMapHits], m_NumObjectMapHits);
	serializer.SerializeInt(g_Properties[PIDX_NumDrawCalls], m_NumDrawCalls);
}
bool EngineStats::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool EngineStats::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumObjectMapHits], &m_NumObjectMapHits , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumDrawCalls], &m_NumDrawCalls , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	return true;
}
void EngineStats::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	EngineStats* other = (EngineStats*) otherObj;
	other->SetNumObjectMapHits(m_NumObjectMapHits);
	other->SetNumDrawCalls(m_NumDrawCalls);
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
