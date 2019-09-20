#include "LXObject.h"

#pragma region  CLASS_SOURCE LXObject

#include "LXObject.h"
#include "serializer.h"
const ClassPropertyData LXObject::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(LXObject, m_ObjectID), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "Name", PIDX_Name, offsetof(LXObject, m_Name), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void LXObject::Serialize(Serializer2& serializer)
{
	serializer.SerializeInt("ObjectID", m_ObjectID);
	serializer.SerializeString("Name", m_Name);
}
bool LXObject::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool LXObject::ShowPropertyGrid()
{
	ImguiHelpers::ShowInt("ObjectID", m_ObjectID , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowString("Name", m_Name  );
	return true;
}
const char* LXObject::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE LXObject

