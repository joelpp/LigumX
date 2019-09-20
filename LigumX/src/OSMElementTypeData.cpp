#include "OSMElementTypeData.h"

#pragma region  CLASS_SOURCE OSMElementTypeData

#include "OSMElementTypeData.h"
#include "serializer.h"
#include "OSMAttribute.h"
const ClassPropertyData OSMElementTypeData::g_Properties[] = 
{
{ "Type", PIDX_Type, offsetof(OSMElementTypeData, m_Type), 0, LXType_Object, sizeof(OSMElementType), LXType_OSMElementType, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "FillIn", PIDX_FillIn, offsetof(OSMElementTypeData, m_FillIn), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugColor", PIDX_DebugColor, offsetof(OSMElementTypeData, m_DebugColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Attributes", PIDX_Attributes, offsetof(OSMElementTypeData, m_Attributes), 0, LXType_stdvector, sizeof(std::vector<OSMAttribute>), LXType_stdvector, false, LXType_OSMAttribute, false, 0, 0, 0, 0,}, 
};
void OSMElementTypeData::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool("FillIn", m_FillIn);
	serializer.SerializeVec3("DebugColor", m_DebugColor);
	serializer.SerializeVector("Attributes", m_Attributes);
}
bool OSMElementTypeData::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementTypeData::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("FillIn", m_FillIn  );
	ImguiHelpers::ShowVec3("DebugColor", m_DebugColor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowVector("Attributes", m_Attributes  );
	return true;
}
const char* OSMElementTypeData::GetTypeName()
{
	return ClassName;
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