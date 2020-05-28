#include "RenderingStats.h"

#pragma region  CLASS_SOURCE RenderingStats

#include "RenderingStats.h"
#include "serializer.h"
const ClassPropertyData RenderingStats::g_Properties[] = 
{
{ "NumVisibleEntities", PIDX_NumVisibleEntities, offsetof(RenderingStats, m_NumVisibleEntities), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "NumVisibleSectors", PIDX_NumVisibleSectors, offsetof(RenderingStats, m_NumVisibleSectors), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "NumDebugModels", PIDX_NumDebugModels, offsetof(RenderingStats, m_NumDebugModels), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Num2DMessages", PIDX_Num2DMessages, offsetof(RenderingStats, m_Num2DMessages), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "fps", PIDX_fps, offsetof(RenderingStats, m_fps), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
void RenderingStats::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_NumVisibleEntities], m_NumVisibleEntities);
	serializer.SerializeInt(g_Properties[PIDX_NumVisibleSectors], m_NumVisibleSectors);
	serializer.SerializeInt(g_Properties[PIDX_NumDebugModels], m_NumDebugModels);
	serializer.SerializeInt(g_Properties[PIDX_Num2DMessages], m_Num2DMessages);
	serializer.SerializeFloat(g_Properties[PIDX_fps], m_fps);
}
bool RenderingStats::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool RenderingStats::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumVisibleEntities], &m_NumVisibleEntities , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumVisibleSectors], &m_NumVisibleSectors , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_NumDebugModels], &m_NumDebugModels , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Num2DMessages], &m_Num2DMessages , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_fps], &m_fps , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
void RenderingStats::Clone(RenderingStats& other)
{
	super::Clone(other);
	other.SetNumVisibleEntities(m_NumVisibleEntities);
	other.SetNumVisibleSectors(m_NumVisibleSectors);
	other.SetNumDebugModels(m_NumDebugModels);
	other.SetNum2DMessages(m_Num2DMessages);
	other.Setfps(m_fps);
}
const char* RenderingStats::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE RenderingStats
