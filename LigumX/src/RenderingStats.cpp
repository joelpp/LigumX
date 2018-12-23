#include "RenderingStats.h"

#pragma region  CLASS_SOURCE RenderingStats

#include "RenderingStats.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData RenderingStats::g_Properties[] = 
{
{ "NumVisibleEntities", PIDX_NumVisibleEntities, offsetof(RenderingStats, m_NumVisibleEntities), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "NumVisibleSectors", PIDX_NumVisibleSectors, offsetof(RenderingStats, m_NumVisibleSectors), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "NumDebugModels", PIDX_NumDebugModels, offsetof(RenderingStats, m_NumDebugModels), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "Num2DMessages", PIDX_Num2DMessages, offsetof(RenderingStats, m_Num2DMessages), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "fps", PIDX_fps, offsetof(RenderingStats, m_fps), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
bool RenderingStats::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool RenderingStats::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_INT("NumVisibleEntities", m_NumVisibleEntities, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_INT("NumVisibleSectors", m_NumVisibleSectors, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_INT("NumDebugModels", m_NumDebugModels, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_INT("Num2DMessages", m_Num2DMessages, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
	LXIMGUI_SHOW_FLOAT("fps", m_fps, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	return true;
}

#pragma endregion  CLASS_SOURCE RenderingStats
