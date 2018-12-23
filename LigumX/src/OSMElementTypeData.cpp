#include "OSMElementTypeData.h"

#pragma region  CLASS_SOURCE OSMElementTypeData

#include "OSMElementTypeData.h"
#include "OSMAttribute.h"
const ClassPropertyData OSMElementTypeData::g_Properties[] = 
{
{ "Type", PIDX_Type, offsetof(OSMElementTypeData, m_Type), 0, LXType_Object, sizeof(OSMElementType), LXType_OSMElementType, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "FillIn", PIDX_FillIn, offsetof(OSMElementTypeData, m_FillIn), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugColor", PIDX_DebugColor, offsetof(OSMElementTypeData, m_DebugColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Attributes", PIDX_Attributes, offsetof(OSMElementTypeData, m_Attributes), 0, LXType_stdvector, sizeof(std::vector<OSMAttribute>), LXType_stdvector, false, LXType_OSMAttribute, false, 0, 0, 0, 0,}, 
};
bool OSMElementTypeData::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementTypeData::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("FillIn", m_FillIn);
	LXIMGUI_SHOW_VEC3("DebugColor", m_DebugColor, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_OBJECT_VECTOR("Attributes", m_Attributes);
	return true;
}

#pragma endregion  CLASS_SOURCE OSMElementTypeData


bool OSMElementTypeData::GetIsBuilding()
{
	for (OSMAttribute& attribute : m_Attributes)
	{
		if (attribute.GetKey() == "building")
		{
			return true;
		}
	}
	
	return false;
}
