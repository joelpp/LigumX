#include "TerrainTool.h"

#pragma region  CLASS_SOURCE TerrainTool

#include "TerrainTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData TerrainTool::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(TerrainTool, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(TerrainTool, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Sector", PIDX_Sector, offsetof(TerrainTool, m_Sector), 0, LXType_Sector, true, LXType_None, 0, 0, 0, }, 
{ "Mode", PIDX_Mode, offsetof(TerrainTool, m_Mode), 0, LXType_TerrainEditionMode, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
};
bool TerrainTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
const std::string EnumValues_TerrainEditionMode[] = 
{
"None",
"Height",
"Color",
};

const TerrainEditionMode Indirection_TerrainEditionMode[] =
{
	TerrainEditionMode_None,
	TerrainEditionMode_Height,
	TerrainEditionMode_Color,
};

#pragma endregion  CLASS_SOURCE TerrainTool



bool TerrainTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (mouseButton1Down)
	{

	}

	return false;
}
