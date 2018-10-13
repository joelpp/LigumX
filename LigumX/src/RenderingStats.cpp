#include "RenderingStats.h"

#pragma region  CLASS_SOURCE RenderingStats

#include "RenderingStats.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData RenderingStats::g_Properties[] = 
{
{ "NumVisibleEntities", PIDX_NumVisibleEntities, offsetof(RenderingStats, m_NumVisibleEntities), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
{ "NumVisibleSectors", PIDX_NumVisibleSectors, offsetof(RenderingStats, m_NumVisibleSectors), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
{ "NumDebugModels", PIDX_NumDebugModels, offsetof(RenderingStats, m_NumDebugModels), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
{ "Num2DMessages", PIDX_Num2DMessages, offsetof(RenderingStats, m_Num2DMessages), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
{ "fps", PIDX_fps, offsetof(RenderingStats, m_fps), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool RenderingStats::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE RenderingStats
