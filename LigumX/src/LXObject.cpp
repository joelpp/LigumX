#include "LXObject.h"

#pragma region  CLASS_SOURCE LXObject

#include "LXObject.h"
#include "serializer.h"
const ClassPropertyData LXObject::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(LXObject, m_ObjectID), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Name", PIDX_Name, offsetof(LXObject, m_Name), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void LXObject::Serialize(Serializer2& serializer)
{
	serializer.SerializeInt(g_Properties[PIDX_ObjectID], m_ObjectID);
	serializer.SerializeString(g_Properties[PIDX_Name], m_Name);
}
bool LXObject::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool LXObject::ShowPropertyGrid()
{
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ObjectID], &m_ObjectID , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_Name], m_Name  );
	return true;
}
const char* LXObject::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE LXObject


